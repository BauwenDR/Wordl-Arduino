package be.kdg.wordle.gui.menu;

import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;

public class MenuView extends VBox {

    private VBox menuOptions;

    private Label lblHead;

    private Button btnWordGame;
    private  Button btnNumberGame;
    private Button btnHistory;

    public MenuView() {
        this.initialiseNodes();
        this.layoutNodes();
    }

    private void initialiseNodes() {
        lblHead = new Label("Game Select");

        menuOptions = new VBox();
        btnWordGame = new Button("Words");
        btnNumberGame = new Button("Numbers");
        btnHistory = new Button("History");
    }

    private void layoutNodes() {
        lblHead.setStyle("-fx-font-size: 60; -fx-font-weight: bold;");

        btnWordGame.setPrefWidth(200);
        btnWordGame.setPrefHeight(35);

        btnNumberGame.setPrefWidth(200);
        btnNumberGame.setPrefHeight(35);

        btnHistory.setPrefWidth(200);
        btnHistory.setPrefHeight(35);

        menuOptions.setSpacing(15);
        menuOptions.setAlignment(Pos.CENTER);
        menuOptions.getChildren().addAll(btnWordGame, btnNumberGame);

        this.setStyle("-fx-background-color:  #CCCCCC");

        this.setSpacing(40);
        this.setAlignment(Pos.CENTER);
        this.getChildren().add(lblHead);
        this.getChildren().add(menuOptions);
        this.getChildren().add(btnHistory);
    }

    public Button getBtnWordGame() {
        return btnWordGame;
    }

    public Button getBtnNumberGame() {
        return btnNumberGame;
    }

    public Button getBtnHistory() {
        return btnHistory;
    }
}
