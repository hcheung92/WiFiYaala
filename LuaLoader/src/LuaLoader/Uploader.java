package LuaLoader;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;

public class Uploader implements Runnable
{
	List<String> commands;
	boolean priorReset;
	
	OutputStream toDevice;
	InputStream fromDevice;
	JTextArea luaRxTa;
	JFrame notificationFrame;
	
	boolean running = true;
	int waitPrompts;
	
	public Uploader(OutputStream toDevice, InputStream fromDevice, JTextArea luaRxTa, JFrame notificationFrame) 
	{
		this.priorReset = false;
		this.toDevice = toDevice;
		this.fromDevice = fromDevice;
		this.luaRxTa = luaRxTa;
		this.commands = commands;
		this.notificationFrame = notificationFrame;
	}
	
	public Uploader(OutputStream toDevice, InputStream fromDevice, JTextArea luaRxTa, boolean priorReset) 
	{
		this.priorReset = priorReset;
		this.toDevice = toDevice;
		this.fromDevice = fromDevice;
		this.luaRxTa = luaRxTa;
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
						dropNextLine = 6;
						priorReset = false;
					}
					else if(rx.toString().endsWith("\r\n"))
					{
						//System.out.println("failed: '"+ rx.toString()+"'" );
						rx = new StringBuilder();
						toDevice.write(" \r\n".getBytes());
						dropNextLine = 1;
					}
				}
				else if(firstPrompt && rx.toString().startsWith(">> "))
				{
//					System.out.println("--hups. we are right in the middle of something. abort--");
					try {
						Thread.sleep(500);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					luaRxTa.append(" --hups. we are right in the middle of something. abort-- ");
					if(notificationFrame != null)
						JOptionPane.showMessageDialog(notificationFrame, "Hups. We are right in the middle of something. Upload failed.");
					toDevice.write(" \r\n".getBytes());
					running = false;
					firstPrompt = false;
				}
				else if(rx.toString().startsWith("> ") || rx.toString().startsWith(">> "))
				{
					if(rx.toString().endsWith("\r\n"))
						System.out.println("oh no...");
					
	           		if(!commands.isEmpty())
            		{
            			String fstr = commands.get(0); 
            			commands.remove(0);
            			System.out.println("send='" +fstr+"'");
            			toDevice.write(fstr.getBytes());
            			toDevice.write(" \r\n".getBytes());
            			line++;
               			dropNextLine = 1;
            		}
        			else
        			{
        				running = false;
        			}
	           		rx = new StringBuilder();
	           		firstPrompt = false;
				}
				else if(rx.toString().startsWith("stdin:") && rx.toString().endsWith("\r\n"))
				{
					System.out.println("--Error in line: "+ line +"--");
					try {
						Thread.sleep(500);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					if(notificationFrame != null)
						JOptionPane.showMessageDialog(notificationFrame, "Unexpected error: " + rx.toString());
					luaRxTa.append(" --Error in line: "+ line +"-- ");
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
