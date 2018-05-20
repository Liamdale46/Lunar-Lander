import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;


public class OrientationDisplay extends JPanel {
    private static final long serialVersionUID = 42l;
    
    private JLabel vXL, vYL;
    private JTextField vXBox, vYBox;
    
    public OrientationDisplay(){
        super( new FlowLayout( FlowLayout.LEFT, 5, 0));
        
        setBorder( BorderFactory.createTitledBorder("Orientation Display"));
        
        vXL = new JLabel();
        vXL.setText("Vx :");
        add(vXL);
        
        vXBox =  new JTextField(20);
        vXBox.setEditable(false);
        add(vXBox);
        
        vYL = new JLabel();
        vYL.setText("Vy :");
        add(vYL);
        
        vYBox =  new JTextField(20);
        vYBox.setEditable(false);
        add(vYBox);
    }
    
    public void setOrientation(int vX, int vY){
        vXBox.setText("" + vX);
        vYBox.setText("" + vY);
    }
}
