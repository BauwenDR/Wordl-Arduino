package be.kdg.wordle.gui.initialise;

import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;

public class InitialiseView extends VBox {

    private Label lblSynchronizing;
    private ImageView imgArduino;
    private ImageView imgLoadingGif;

    public InitialiseView() {
        this.initialiseNodes();
        this.layoutNodes();
    }

    private void initialiseNodes() {
        lblSynchronizing = new Label("Connecting to Arduino...");
        imgArduino = new ImageView(new Image(("images/arduino-syncPage.png")));
        imgLoadingGif = new ImageView(new Image("images/loading-syncPage.gif"));
    }

    private void layoutNodes() {
        imgArduino.setPreserveRatio(true);
        imgLoadingGif.setPreserveRatio(true);

        imgArduino.setFitWidth(400);
        imgLoadingGif.setFitWidth(100);

        lblSynchronizing.setStyle("-fx-font-size: 50; -fx-font-weight: bold;");

        this.setStyle("-fx-background-color:  #CCCCCC");

        this.setAlignment(Pos.CENTER);
        this.setSpacing(30);

        this.getChildren().add(lblSynchronizing);
        this.getChildren().add(imgArduino);
        this.getChildren().add(imgLoadingGif);
    }
}
