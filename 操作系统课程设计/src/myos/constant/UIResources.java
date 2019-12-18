package myos.constant;

import javafx.scene.Node;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;


public class UIResources {
    public static Node getDirectoryIcon(){
     return    new ImageView(new Image(UIResources.class.getResourceAsStream("/ui/directory.png")));
    }
    public static Node getFileIcon(){
      return   new ImageView(new Image(UIResources.class.getResourceAsStream("/ui/file.png")));
    }
    public static Node getProgramIcon(){
        return new ImageView(new Image(UIResources.class.getResourceAsStream("/ui/program.png")));
    }
}
