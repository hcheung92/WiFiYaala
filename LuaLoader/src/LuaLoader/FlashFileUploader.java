package LuaLoader;

import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.swing.JFrame;
import javax.swing.JTextArea;

public class FlashFileUploader extends Uploader
{
	
	public FlashFileUploader(OutputStream toDevice, InputStream fromDevice, JTextArea luaRxTa, String fileName, String flashName, JFrame notificationFrame) 
	{
		super(toDevice, fromDevice, luaRxTa, notificationFrame);

		BufferedReader runFileBufferedReader;
		commands = new ArrayList<String>();

		if(flashName == null || flashName.length() == 0)
			flashName = fileName.split("/")[fileName.split("/").length-1];
		
		commands.add("file.open(\"" + flashName + "\",\"w+\")");
		try {
			runFileBufferedReader = new BufferedReader(new FileReader(fileName));
			
			String line;
			while((line = runFileBufferedReader.readLine()) != null)
			{
				commands.add("file.writeline([[" + line + "]])");
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
		
		commands.add("file.close()");
		
		for(int i=0; i<commands.size(); i++)
			System.out.println(commands.get(i));
		
	}

}
