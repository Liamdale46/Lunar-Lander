import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;

public class AltitudeDisplay extends JPanel {
    private static final long serialVersionUID = 42l;
    
    private JLabel title, status;
    private ImageIcon red, green, grey, orange;
    private JTextField altBox;

    public AltitudeDisplay() {
        super( new FlowLayout( FlowLayout.LEFT, 5, 0));
        
        setBorder( BorderFactory.createTitledBorder("Altitude Display"));
        
        red = new ImageIcon(getClass().getResource("led-red.png"));
        green = new ImageIcon(getClass().getResource("led-green.png"));
        grey = new ImageIcon(getClass().getResource("led-grey.png"));
        orange = new ImageIcon(getClass().getResource("led-orange.png"));
        
        title = new JLabel();
        title.setText("Altitude :");
        add(title);
        
        altBox =  new JTextField(20);
        altBox.setEditable(false);
        add(altBox);
        
        status = new JLabel();
        add(status);
        status.setIcon(grey);
    }

    public void setAltitude(float alt, boolean fly) {
        altBox.setText("" + alt);
        
        if(alt >= 60.00 && fly == true){
            status.setIcon(green);
        }
        else if(alt <= 59.99 && alt >= 30.00 && fly == true){
            status.setIcon(orange);
        }
        else if(alt <= 29.99 && alt >= 0 && fly == true){
            status.setIcon(red);
        }
        else if(fly == false){
            status.setIcon(red);
            altBox.setText("GAME OVER");
        }
    }
    
}
