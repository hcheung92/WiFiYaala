package LuaLoader;

import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.swing.JTextArea;

public class RunFileUploader extends Uploader
{
	
	public RunFileUploader(OutputStream toDevice, InputStream fromDevice, JTextArea luaRxTa, String fileName, boolean priorReset) 
	{
		super(toDevice, fromDevice, luaRxTa, priorReset);

		BufferedReader runFileBufferedReader;
		super.commands = new ArrayList<String>();

		try {
			runFileBufferedReader = new BufferedReader(new FileReader(fileName));
			
			String line;
			while((line = runFileBufferedReader.readLine()) != null)
			{
				super.commands.add(line);
			}
			
		} 
		catch (FileNotFoundException e1) 
		{
			e1.printStackTrace();
			luaRxTa.append(e1.getMessage());
			running = false;
		}
		catch (IOException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

}
