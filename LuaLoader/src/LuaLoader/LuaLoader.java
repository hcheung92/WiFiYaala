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
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.Reader;
import java.nio.Buffer;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.JTree;
import javax.swing.text.DefaultCaret;
import javax.swing.tree.DefaultMutableTreeNode;

public class LuaLoader extends JFrame implements ActionListener, SerialPortEventListener{

	int width = 1024;
	int height = 800;

	String[] serialPortsArray;
	
	//setup
	JButton portRefreshBtn;
	JComboBox serialPortCombox;
	JToggleButton connectTbtn;
	JCheckBox rtsChkbox;
	JCheckBox dtrChkbox;
	
	SerialCom serial = new SerialCom();

	Uploader uploader;
	PipedOutputStream toUploaderPipe;
	Thread uploadThread;
	
	//ram	
	JTextArea luaRxTa;
	JScrollPane luaRxSta;
	JTextField luaCmdTf;
	JButton selectRunFileBtn;
	final JFileChooser fc = new JFileChooser();
	JTextField runFileTf;
	JButton runFileBtn;
	JCheckBox priorResetChkbox;

	
	//file
	JButton uploadFileBtn;
	JButton refreshFileBtn;
	JLabel flashFileName;
	JTree fileTree;
	JTextArea flashFileTa;
	JLabel fileSizeLabel;
	JButton updateFileBtn;
	JButton saveFileBtn;
	JButton delFileBtn;
	
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
		selectRunFileBtn = new JButton("Select a File");
		selectRunFileBtn.addActionListener(this);
		fc.addChoosableFileFilter(new OpenFileFilter("lua","lua Files") );
		c.gridx=0;
		c.gridy=0;
		ramPanel.add(selectRunFileBtn, c);
		
		runFileTf = new JTextField(30);
		runFileTf.setText("/Users/sid/Desktop/test.lua");												//DEBUG
		c.gridx=1;
		c.gridy=0;
		c.gridwidth=2;
		ramPanel.add(runFileTf, c);

		runFileBtn = new JButton("Run File");
		runFileBtn.addActionListener(this);
		runFileBtn.setEnabled(false);
		c.gridx=3;
		c.gridy=0;
		c.gridwidth=1;
		c.insets = new Insets(10, 10, 10, 3);
		ramPanel.add(runFileBtn, c);
		
		priorResetChkbox = new JCheckBox("start clean (reset)");
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
		luaRxTa.setEditable(false);
		luaRxSta = new JScrollPane(luaRxTa); 
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
		uploadFileBtn = new JButton("Upload File");
		uploadFileBtn.setEnabled(false);
		uploadFileBtn.addActionListener(this);
		c.gridx=0;
		c.gridy=0;
		c.gridwidth=1;
		filePanel.add(uploadFileBtn, c);

		refreshFileBtn = new JButton("Refresh");
		refreshFileBtn.addActionListener(this);
		c.gridx=1;
		c.gridy=0;
		filePanel.add(refreshFileBtn, c);

		c.gridx=2;
		c.gridy=0;
		filePanel.add(new JLabel("Flash file:"), c);
		
		c.gridx=3;
		c.gridy=0;
		c.gridwidth=2;
		flashFileName = new JLabel("none");
		filePanel.add(flashFileName, c);
				
		c.gridx=0;
		c.gridy=1;
		c.gridwidth=2;
	    DefaultMutableTreeNode top = new DefaultMutableTreeNode("Flash");
		fileTree = new JTree(top);
		//TODO listener
		JScrollPane fileSTree = new JScrollPane(fileTree);
		filePanel.add(fileSTree, c);
		
		flashFileTa = new JTextArea(20, 50);
		flashFileTa.setLineWrap(true);
		flashFileTa.setWrapStyleWord(true);
		JScrollPane flashFileSTa = new JScrollPane(flashFileTa); 
		flashFileTa.setEditable(true);
		c.gridx=2;
		c.gridy=1;
		c.gridwidth=3;
		c.insets = new Insets(10, 10, 0, 10);
		filePanel.add(flashFileSTa, c);
		
		c.gridx=0;
		c.gridy=3;
		c.gridwidth=1;
		filePanel.add(new JLabel("Size:"), c);

		fileSizeLabel = new JLabel("");
		c.gridx=1;
		c.gridy=3;
		filePanel.add(fileSizeLabel, c);
		
		saveFileBtn = new JButton("Save to disk");
		saveFileBtn.addActionListener(this);
		c.gridx=3;
		c.gridy=3;
		c.gridwidth=1;
		filePanel.add(saveFileBtn, c);
		
		delFileBtn = new JButton("Delete");
		delFileBtn.addActionListener(this);
		c.gridx=2;
		c.gridy=3;
		c.gridwidth=1;
		filePanel.add(delFileBtn, c);
		
		updateFileBtn = new JButton("Update file");
		updateFileBtn.addActionListener(this);
		c.gridx=4;
		c.gridy=3;
		c.gridwidth=1;
		filePanel.add(updateFileBtn, c);
		
		///
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
		{																					//TODO telnet
			if(connectTbtn.isSelected())		//connect
			{
				try 
				{
					serial.connect((String)serialPortCombox.getSelectedItem(), rtsChkbox.isSelected(), dtrChkbox.isSelected());
					connected();
				} catch (Exception e1) 
				{
					disconnected();
					connectTbtn.setSelected(false);
					e1.printStackTrace();
				}
			}
			else								//disconnect
			{
				try {
					serial.disconnect();
				}
				catch (IOException e1) 
				{
					connectTbtn.setSelected(false);
					e1.printStackTrace();
				}
				finally
				{
					disconnected();
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
			if(connectTbtn.isSelected())
			{
				try {													//todo telnet, connection wrapper??
					serial.out.write(luaCmdTf.getText().getBytes());
					serial.out.write("\r\n".getBytes());
					luaCmdTf.setText("");
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		}
		else if(e.getSource() == selectRunFileBtn)
		{
			int returnVal = fc.showOpenDialog(this);
			
	        if (returnVal == JFileChooser.APPROVE_OPTION) 
	        {
	           runFileTf.setText(fc.getSelectedFile().getAbsolutePath());
	        }
		}
		else if(e.getSource() == runFileBtn)
		{
			if(connectTbtn.isSelected())
			{
				//ram case
				if(uploadThread != null && uploader != null)
				{
					uploader.terminate();
					try {
						toUploaderPipe.write("\r\n".getBytes());
					} catch (IOException e1) {
						//e1.printStackTrace();
					}
				}

				
				toUploaderPipe = new PipedOutputStream();
				try {
					uploader = new RunFileUploader(serial.out, new PipedInputStream(toUploaderPipe), luaRxTa, runFileTf.getText(), priorResetChkbox.isSelected());
					uploadThread = new Thread(uploader);
					uploadThread.start();
				} catch (IOException e2) {
					e2.printStackTrace();
				}
			}
		}
		else if(e.getSource() == uploadFileBtn)
		{
			int returnVal = fc.showOpenDialog(this);
			
	        if (returnVal == JFileChooser.APPROVE_OPTION) 
	        {
	        	
	        	String flashName = JOptionPane.showInputDialog(this, "Change name in flash to: (leave blank or press cancel if not)");
	        	
				if(uploadThread != null && uploader != null)
				{
					if(uploadThread.isAlive())
					{
						JOptionPane.showMessageDialog(this, "Another thread seems to be running");
						return;
					}
				}

				
				toUploaderPipe = new PipedOutputStream();
				try {
					uploader = new FlashFileUploader(serial.out, new PipedInputStream(toUploaderPipe), luaRxTa, fc.getSelectedFile().getAbsolutePath(), flashName, this);
					uploadThread = new Thread(uploader);
					uploadThread.start();
				} catch (IOException e2) {
					e2.printStackTrace();
				}
	        	
	        }
		}
		else
		{
			System.out.println("Unknown action from:" + e.getSource().toString());
			
		}
		
	}


	char lastAdded = 0;
	@Override
	public synchronized void serialEvent(SerialPortEvent arg0) 
	{
        try
        {
 //           System.out.print("Start ");
            byte[] data = new byte[serial.in.available()*2];
            int read = serial.in.read(data);
            String sentence = new String(data, 0, read,"UTF-8");

            if(toUploaderPipe != null && uploader != null && uploadThread != null && uploadThread.isAlive())
            {
 //           	System.out.println("to fileuploader: '" + sentence+"' length:" + sentence.length());
            	toUploaderPipe.write(data,0 ,read);
            	toUploaderPipe.flush();
            }
            

            //avoid blank lines
            for(char ch : sentence.toCharArray())
            {
            	
            	if(lastAdded == '\n' && (ch == '\r' || ch == '\n'))
            		continue;
            	luaRxTa.append(ch + "");
            	lastAdded = ch;
            		
            }
            //luaRxTa.append(sentence);

        }
        catch ( IOException e )
        {
            e.printStackTrace();
            System.exit(-1);
        } 	
	}
	
	void connected()
	{
		runFileBtn.setEnabled(true);
		uploadFileBtn.setEnabled(true);
	}
	
	void disconnected()
	{
		runFileBtn.setEnabled(false);
		uploadFileBtn.setEnabled(false);
	}
}
