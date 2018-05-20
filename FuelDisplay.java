import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;

public class FuelDisplay extends JPanel {
    private static final long serialVersionUID = 42l;

    private JLabel title, status;
    private ImageIcon red, green, grey, orange;
    private JTextField percentBox;
    public FuelDisplay() {
        super( new FlowLayout( FlowLayout.LEFT, 5, 0));
        
        setBorder( BorderFactory.createTitledBorder("Fuel Display"));
        
        red = new ImageIcon(getClass().getResource("led-red.png"));
        green = new ImageIcon(getClass().getResource("led-green.png"));
        grey = new ImageIcon(getClass().getResource("led-grey.png"));
        orange = new ImageIcon(getClass().getResource("led-orange.png"));
        
        title = new JLabel();
        title.setText("Fuel :");
        add(title);
       
        percentBox =  new JTextField(20);
        percentBox.setEditable(false);
        add(percentBox);
        
        status = new JLabel();
        add(status);
        status.setIcon(grey);
    }

    public void setFuel(float percent, boolean fly) {
        String text =  "" + percent;
        percentBox.setText(text);
        if (percent <= 100.00 && percent >= 70.00 && fly == true){
            status.setIcon(green);
        }
        else if (percent <= 69.99 && percent >= 30.00 && fly == true ){
            status.setIcon(orange);
        }
        else if (percent <= 29.99 && percent >= 0 && fly == true){
            status.setIcon(red);
        }
        else if (fly == false){
            status.setIcon(red);
            percentBox.setText("GAME OVER");
        }
    }
}
