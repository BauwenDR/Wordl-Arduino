package be.kdg.wordle.gui.game;

import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;

public class GameView extends VBox {

    private Label lblTitle;
    private GridPane gameGrid;
    private Label[][] inputFields;

    private Label lblDescription;

    public GameView() {
        this.initialiseNodes();
        this.layoutNodes();
    }

    public void addDescription(String gamemode) {
        lblDescription.setText("Fill in on the Arduino or type a " + gamemode);
    }

    private void initialiseNodes() {
        lblTitle = new Label("Wordle");

        gameGrid = new GridPane();
        inputFields = new Label[4][4];

        for(int i = 0; i  < inputFields.length; i++) {
           for(int j = 0; j < inputFields[i].length; j++) {
               inputFields[i][j] = new Label();
               inputFields[i][j].setId("gridItem");
           }
        }
        lblDescription = new Label();
    }

    private void layoutNodes() {

        lblTitle.setStyle("-fx-font-size: 60; -fx-font-weight: bold");

        for(int i = 0; i  < inputFields.length; i++) {
            for(int j = 0; j < inputFields[i].length; j++) {
                inputFields[i][j].setAlignment(Pos.CENTER);
                gameGrid.add(inputFields[i][j], i, j);
            }
        }

        lblDescription.setStyle("-fx-font-size: 15");

        gameGrid.setAlignment(Pos.CENTER);
        gameGrid.setVgap(-2);   //otherwise the label border don't overlap, thus making them thicker
        gameGrid.setHgap(-2);

        this.setAlignment(Pos.CENTER);
        this.setSpacing(30);

        this.setStyle("-fx-background-color:  #CCCCCC");

        this.getChildren().add(lblTitle);
        this.getChildren().add(gameGrid);
        this.getChildren().add(lblDescription);
    }

    public Label[][] getInputFields() {
        return inputFields;
    }
}
