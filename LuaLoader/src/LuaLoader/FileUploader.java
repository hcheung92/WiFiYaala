package LuaLoader;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.swing.JTextArea;

public class FileUploader implements Runnable
{
	BufferedReader runFileBufferedReader;
	boolean priorReset;
	
	OutputStream toDevice;
	InputStream fromDevice;
	JTextArea luaRxTa;
	
	boolean running = true;
	int waitPrompts;
	
	public FileUploader(OutputStream toDevice, InputStream fromDevice, JTextArea luaRxTa, String fileName, boolean priorReset) 
	{
		this.priorReset = priorReset;
		this.toDevice = toDevice;
		this.fromDevice = fromDevice;
		this.luaRxTa = luaRxTa;
		
		try {
			runFileBufferedReader = new BufferedReader(new FileReader(fileName));
		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
			luaRxTa.append(e1.getMessage());
			return;
		}
	}

	@Override
	public void run()
	{
		boolean firstPrompt;
		int dropNextLine,line = 0;
		
		try 
		{
			while(fromDevice.available() > 0)
				fromDevice.read();
			
			toDevice.write(" \r\n".getBytes());
			dropNextLine = 2;
			firstPrompt = true;
			
			StringBuilder rx = new StringBuilder();
			
			while(running)
			{
				
				rx.append((char) fromDevice.read());
//				if(rx.toString().endsWith("\r\n"))
//					System.out.println("rx='" + rx.toString() + "'");
				
				if(dropNextLine > 0)
				{
					if(rx.toString().endsWith("\r\n"))
					{
						rx = new StringBuilder();
						dropNextLine--;
//						System.out.println("Line dropped");
					}
					continue;
				}
				
				if(priorReset)
				{
					if(rx.toString().startsWith(">> "))
					{
						toDevice.write("end\r\n".getBytes());
						dropNextLine = 1;
					}
					else if(rx.toString().startsWith("> "))
					{
						toDevice.write("node.restart()\r\n".getBytes());
						dropNextLine = 4;
						priorReset = false;
					}
					else
					{
						System.out.println("failed: '"+ rx.toString()+"'" );
					}
				}
				else if(firstPrompt && rx.toString().startsWith(">> "))
				{
//					System.out.println("--hups. we are right in the middle of something. abort--");
					luaRxTa.append("--hups. we are right in the middle of something. abort--\r\n");
					toDevice.write(" \r\n".getBytes());
					running = false;
					firstPrompt = false;
				}
				else if(rx.toString().startsWith("> ") || rx.toString().startsWith(">> "))
				{
					if(rx.toString().endsWith("\r\n"))
						System.out.println("oh no...");
					
	           		if(runFileBufferedReader != null)
            		{
            			String fstr = runFileBufferedReader.readLine();
            			if(fstr != null)
            			{
            				System.out.println("send='" +fstr+"'");
            				toDevice.write(fstr.getBytes());
            				toDevice.write(" \r\n".getBytes());
            				line++;
               				dropNextLine = 1;
            			}
            			else
            			{
            				runFileBufferedReader.close();
            				runFileBufferedReader = null;
            				running = false;
            			}
            		}
	           		rx = new StringBuilder();
	           		firstPrompt = false;
				}
				else if(rx.toString().startsWith("stdin:") && rx.toString().endsWith("\r\n"))
				{
					System.out.println("--Error in line: "+ line +"--");
					luaRxTa.append("--Error in line: "+ line +"--\r\n");
					toDevice.write(" \r\n".getBytes());
					running = false;
				}
				else
				{
					if(rx.toString().endsWith("\r\n"))
					{
						//System.out.println("Unknown string:'" + rx.toString() + "'");
						//System.out.println("dropping");
						rx = new StringBuilder();
					}
				}
				toDevice.flush();

			}
				
		} 
		catch (IOException e1) 
		{
			e1.printStackTrace();
		}
	}
	
	public void terminate()
	{
		running = false;
	}
}
