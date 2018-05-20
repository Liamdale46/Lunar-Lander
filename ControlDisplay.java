import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;

public class ControlDisplay extends JPanel {
    private static final long serialVersionUID = 42l;

    private JLabel title;
    private JTextField feedback;

    public ControlDisplay(){
        super( new FlowLayout( FlowLayout.LEFT, 5, 0));

        setBorder( BorderFactory.createTitledBorder("Current Control Mode"));

        title = new JLabel();
        title.setText("Control mode :");
        add(title);

        feedback =  new JTextField(40);
        feedback.setEditable(false);
        add(feedback);

    }

    public void setMode(boolean automatic){
        if(automatic){
            feedback.setText("Automatic, use accelorometer and potentiometer");
        }
        else{
            feedback.setText("manual, use joystick and buttons");
        }
    }
}
