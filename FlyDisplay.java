import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;


public class FlyDisplay extends JPanel {
    private static final long serialVersionUID = 42l;
    
    private JLabel title, status;
    private ImageIcon red, green, grey;
    private JTextField feedback;
    
    public FlyDisplay(){
        super( new FlowLayout( FlowLayout.LEFT, 5, 0));
        
        setBorder( BorderFactory.createTitledBorder("Current Lander Status"));
        
        red = new ImageIcon(getClass().getResource("led-red.png"));
        green = new ImageIcon(getClass().getResource("led-green.png"));
        grey = new ImageIcon(getClass().getResource("led-grey.png"));
        
        title = new JLabel();
        title.setText("Status :");
        add(title);
        
        feedback =  new JTextField(20);
        feedback.setEditable(false);
        add(feedback);
        
        status = new JLabel();
        add(status);
        status.setIcon(grey);
    }
    
    public void setFly(boolean flying){
        if(flying){
            feedback.setText("Flying");
            status.setIcon(green);
        }
        else{
            feedback.setText("GAME OVER");
            status.setIcon(red);
        }
    }
}
