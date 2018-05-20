import java.awt.*;
import javax.swing.text.*;
import javax.swing.border.*;
import java.awt.event.*;
import javax.swing.*;
import java.net.*;
import java.io.*;

// ip-address and port number placeholder
public class DatagramPanel extends JPanel {
    public static final long serialVersionUID = 2L;

    JTextField addressname, port;
 // JPanel created with border and text fields 
    public DatagramPanel() {
        super( new FlowLayout( FlowLayout.LEFT, 5, 0));
        setBorder( BorderFactory.createTitledBorder("Socket Address"));
        add(new JLabel("IP:"));
        addressname = new JTextField(10);
        add(this.addressname);
        add(new JLabel("port:"));
        port = new JTextField(5);
        add(this.port);
    }
    void setAddress(InetSocketAddress where){
        addressname.setText(where.getHostString());
        port.setText( Integer.toString(where.getPort()) );
    }
}
