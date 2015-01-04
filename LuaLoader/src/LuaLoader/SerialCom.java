package LuaLoader;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

public class SerialCom implements SerialPortEventListener 
{
	
    InputStream in;
    OutputStream out;
    SerialPort serialPort;
    SerialPortEventListener listener;

    static String[] listPorts()
    {
    	List<String> myList = new ArrayList<String>();
    	
        java.util.Enumeration<CommPortIdentifier> portEnum = CommPortIdentifier.getPortIdentifiers();
        while ( portEnum.hasMoreElements() ) 
        {
            CommPortIdentifier portIdentifier = portEnum.nextElement();
            myList.add(portIdentifier.getName());
            //System.out.println(portIdentifier.getName()  +  " - " +  getPortTypeName(portIdentifier.getPortType()) );
        }   
        return myList.toArray(new String[myList.size()]);
    }
    
    static String getPortTypeName ( int portType )
    {
        switch ( portType )
        {
            case CommPortIdentifier.PORT_I2C:
                return "I2C";
            case CommPortIdentifier.PORT_PARALLEL:
                return "Parallel";
            case CommPortIdentifier.PORT_RAW:
                return "Raw";
            case CommPortIdentifier.PORT_RS485:
                return "RS485";
            case CommPortIdentifier.PORT_SERIAL:
                return "Serial";
            default:
                return "unknown type";
        }
    }
    
    void addEventListener(SerialPortEventListener list)
    {
    	listener = list;
    }
    
    boolean connect ( String portName, boolean rts, boolean dtr) throws Exception
    {
        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
        if ( portIdentifier.isCurrentlyOwned() )
        {
            throw new Exception("Error: Port is currently in use");
        }
        else
        {
            CommPort commPort = portIdentifier.open(this.getClass().getName(),2000);
            
            if ( commPort instanceof SerialPort )
            {
                serialPort = (SerialPort) commPort;
                serialPort.setRTS(rts);
                serialPort.setDTR(dtr);
                serialPort.setSerialPortParams(115200,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
                serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);

            
                in = serialPort.getInputStream();
                out = serialPort.getOutputStream();

                serialPort.addEventListener(this);
                serialPort.notifyOnDataAvailable(true);
            }
            else
            {
                throw new Exception("Error: Only serial ports are handled by this example.");
            }
            return true;
        }     
    }
    
    
    public void disconnect() throws IOException
    {
    	if(in != null)
    		in.close();
    	if(out != null)
    		out.close();
    	if(serialPort != null)
    	{
    		try
    		{
    			serialPort.close();
    		}
    		catch(Exception e)
    		{
    			
    		}
    	}
    }
    
	@Override
	public void serialEvent(SerialPortEvent arg0) 
	{
			if(listener != null)
				listener.serialEvent(arg0);
			else
				System.err.println("Serial event listener unset");
	}

}
