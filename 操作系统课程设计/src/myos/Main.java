package myos;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.fxml.JavaFXBuilderFactory;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import myos.controller.MainController;


public class Main extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception {
        FXMLLoader fxmlLoader=new FXMLLoader();
        fxmlLoader.setLocation(getClass().getResource("/os.fxml"));
        fxmlLoader.setBuilderFactory(new JavaFXBuilderFactory());
        Parent root = fxmlLoader.load();
        MainController mainController= fxmlLoader.getController();
        OS os=OS.getInstance();
        os.setMainController(mainController);
        mainController.setOs(os);
        //设置标题大小等等
        primaryStage.setTitle("操作系统课程设计");
        primaryStage.setScene(new Scene(root,1000,580));
        primaryStage.setResizable(false);
        //操作系统的关闭事件
        primaryStage.setOnCloseRequest(new EventHandler<WindowEvent>() {
            @Override
            public void handle(WindowEvent event) {
                mainController.closeOs();
                System.exit(0);
            }
        });

        primaryStage.show();
    }

    public static void main(String[] args) throws Exception {
               launch(args);
    }
}
