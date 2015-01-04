package LuaLoader;

import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.text.DefaultCaret;

public class LuaLoader extends JFrame implements ActionListener, SerialPortEventListener{

	int width = 1024;
	int height = 800;

	String[] serialPortsArray;
	
	
	JButton portRefreshBtn;
	JComboBox serialPortCombox;
	JTextArea luaRxTa;
	JScrollPane luaRxSta;
	JTextField luaCmdTf;
	
	JToggleButton connectTbtn;
	JCheckBox rtsChkbox;
	JCheckBox dtrChkbox;
	
	SerialCom serial = new SerialCom();
	
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 2694567418531282387L;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					LuaLoader frame = new LuaLoader();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public LuaLoader() {
		
		serialPortsArray= serial.listPorts();
		serial.addEventListener(this);
		
		setTitle("Lua Loader v0.1");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
		setBounds((d.width - width) /2, (d.height - height) /2, width, height);

		JPanel panel = new JPanel(new GridBagLayout());
		getContentPane().add(panel, BorderLayout.NORTH);

		GridBagConstraints c = new GridBagConstraints();
		c.insets = new Insets(10, 10, 10, 10);
		
		JTabbedPane tabConnection = new JTabbedPane();
		
		JPanel serialPanel = new JPanel(new GridBagLayout());
		JPanel telnetPanel = new JPanel(new GridBagLayout());
		
		///Serial
		c.gridy=0;
		c.gridx=0;
		serialPanel.add(new JLabel("Port:"), c); 
		serialPortCombox = new JComboBox(serialPortsArray);
		c.gridx=1;
		c.gridy=0;
		serialPanel.add(serialPortCombox, c);
		rtsChkbox = new JCheckBox("RTS");
		rtsChkbox.addActionListener(this);
		c.gridx=2;
		c.gridy=0;
		serialPanel.add(rtsChkbox, c);
		dtrChkbox = new JCheckBox("DTR");
		dtrChkbox.addActionListener(this);
		c.gridx=3;
		c.gridy=0;
		serialPanel.add(dtrChkbox, c);
		
		portRefreshBtn = new JButton("Rescan");
		portRefreshBtn.addActionListener(this);
		c.gridx=4;
		c.gridy=0;
		serialPanel.add(portRefreshBtn, c);
		
		
		//telnet
		c.gridy=0;
		c.gridx=0;
		telnetPanel.add(new JLabel("Host:"), c); 

		JTextField hostTf = new JTextField(15);
		c.gridx=1;
		c.gridy=0;
		telnetPanel.add(hostTf, c);
		
		c.gridx=2;
		c.gridy=0;
		telnetPanel.add(new JLabel("Port:"), c); 

		JTextField hostPortTf = new JTextField(5);
		c.gridx=3;
		c.gridy=0;
		telnetPanel.add(hostPortTf, c);
		

		tabConnection.add("Serial", serialPanel);
		tabConnection.add("Telnet", telnetPanel);
			
		c.gridx=0;
		c.gridy=0;
		c.gridwidth=2;
		panel.add(tabConnection, c);
		
		connectTbtn = new JToggleButton("Connect");
		connectTbtn.addActionListener(this);
		c.gridx=2;
		c.gridy=0;
		c.gridwidth=1;
		panel.add(connectTbtn, c);
		

		
		
		JTabbedPane tabWork = new JTabbedPane();
		
		JPanel ramPanel = new JPanel(new GridBagLayout());
		JPanel filePanel = new JPanel(new GridBagLayout());

		//RAM
		JButton selectRunFileBtn = new JButton("Select File");
		c.gridx=0;
		c.gridy=0;
		ramPanel.add(selectRunFileBtn, c);
		
		JTextField runFileTf = new JTextField(30);
		c.gridx=1;
		c.gridy=0;
		c.gridwidth=2;
		ramPanel.add(runFileTf, c);

		JButton runFileBtn = new JButton("Run File");
		c.gridx=3;
		c.gridy=0;
		c.gridwidth=1;
		c.insets = new Insets(10, 10, 10, 3);
		ramPanel.add(runFileBtn, c);
		
		JCheckBox priorResetChkbox = new JCheckBox("start clean (reset)");
		c.gridx=4;
		c.gridy=0;
		c.insets = new Insets(10, 3, 10, 10);
		ramPanel.add(priorResetChkbox, c);
		
		c.insets = new Insets(10, 10, 10, 10);
		
		luaRxTa = new JTextArea(20, 70);
		DefaultCaret caret = (DefaultCaret) luaRxTa.getCaret();
	    caret.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);
	    luaRxTa.setLineWrap(true);
	    luaRxTa.setWrapStyleWord(true);
		luaRxSta = new JScrollPane(luaRxTa); 
		luaRxTa.setEditable(false);
		c.gridx=0;
		c.gridy=1;
		c.gridwidth=5;
		c.insets = new Insets(10, 10, 0, 10);
		ramPanel.add(luaRxSta, c);
		
		luaCmdTf = new JTextField(70);
		luaCmdTf.addActionListener(this);
		c.gridx=0;
		c.gridy=2;
		c.insets = new Insets(0, 10, 10, 10);
		ramPanel.add(luaCmdTf, c);
	
		c.insets = new Insets(10, 10, 10, 10);

		
		//FILE
		c.gridx=0;
		c.gridy=0;
		c.gridwidth=1;
		filePanel.add(new JLabel("File Browser... TODO"), c);
		
		JButton uploadFileBtn = new JButton("Upload File");
		c.gridx=0;
		c.gridy=1;
		filePanel.add(uploadFileBtn, c);
		
		
		c.gridx=0;
		c.gridy=1;
		c.gridwidth=3;
		panel.add(tabWork, c);


		tabWork.add("RAM", ramPanel);
		tabWork.add("Filesystem", filePanel);		
		
		
		
		
		setVisible(true);

	
	
	}

	@Override
	public void actionPerformed(ActionEvent e) 
	{
		if(e.getSource() == portRefreshBtn)				//refresh button
		{
			serialPortsArray = SerialCom.listPorts();
			serialPortCombox.removeAllItems();
		    for(String s:serialPortsArray)
		    {
		        serialPortCombox.addItem(s);
		    }
		}
		else if(e.getSource() == connectTbtn)			//connect button
		{
			if(connectTbtn.isSelected())		//connect
			{
				try 
				{
					serial.connect((String)serialPortCombox.getSelectedItem(), rtsChkbox.isSelected(), dtrChkbox.isSelected());
				} catch (Exception e1) 
				{
					connectTbtn.setSelected(false);
					e1.printStackTrace();
				}
			}
			else								//disconnect
			{
				try {
					serial.disconnect();
				} catch (IOException e1) {
					connectTbtn.setSelected(false);
					e1.printStackTrace();
				}
			}
			
		}
		else if(e.getSource() == rtsChkbox)
		{
			if(serial.serialPort != null)
			{
				serial.serialPort.setRTS(rtsChkbox.isSelected());
				System.out.println("RTS:" + rtsChkbox.isSelected());
			}
		}
		else if(e.getSource() == dtrChkbox)
		{
			if(serial.serialPort != null)
			{
				serial.serialPort.setDTR(dtrChkbox.isSelected());
				System.out.println("DTR:" + dtrChkbox.isSelected());
			}
		}
		else if(e.getSource() == luaCmdTf)
		{
			try {
				serial.out.write(luaCmdTf.getText().getBytes());
				serial.out.write("\r\n".getBytes());
				luaCmdTf.setText("");
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			
		}
		else
		{
			System.out.println("Unknown action from:" + e.getSource().toString());
			
		}
		
	}

	
	@Override
	public synchronized void serialEvent(SerialPortEvent arg0) 
	{
        try
        {
            System.out.print("Start ");
            byte[] data = new byte[serial.in.available()*2];
            int read = serial.in.read(data);
            String sentence = new String(data, "UTF-8");
            
            luaRxTa.append(sentence);

        }
        catch ( IOException e )
        {
            e.printStackTrace();
            System.exit(-1);
        } 	
	}
}
