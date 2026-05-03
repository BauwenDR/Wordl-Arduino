package be.kdg.wordle.gui.connected;

import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;

public class ConnectedView extends VBox {

    private Label lblConnected;
    private ImageView imgArduino;
    private Button btnStart;

    public ConnectedView() {
        this.initialiseNodes();
        this.layoutNodes();
    }

    private void initialiseNodes() {
        lblConnected = new Label("Connected to Arduino");
        imgArduino = new ImageView(new Image("images/arduino-syncPage.png"));
        btnStart = new Button("Start game");
    }

    private void layoutNodes() {
        lblConnected.setStyle("-fx-font-size: 50; -fx-font-weight: bold;");

        imgArduino.setPreserveRatio(true);
        imgArduino.setFitWidth(400);

        btnStart.setPrefWidth(200);
        btnStart.setPrefHeight(35);

        this.setStyle("-fx-background-color:  #CCCCCC");

        this.setAlignment(Pos.CENTER);
        this.setSpacing(35);

        this.getChildren().add(lblConnected);
        this.getChildren().add(imgArduino);
        this.getChildren().add(btnStart);
    }

    public Button getBtnStart() {
        return btnStart;
    }
}
