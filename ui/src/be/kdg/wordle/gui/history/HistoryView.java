package be.kdg.wordle.gui.history;

import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;

import java.util.LinkedList;

public class HistoryView extends VBox {

    private Label lblHeader;

    private GridPane historyGrid;

    private Button btnClode;

    public HistoryView() {
        this.initialiseNodes();
        this.layoutNodes();
    }

    public void fillHistory(LinkedList<String[]> history) {
        for(int i = 0; i < 10 && i < history.size(); i++) {
            for(int j  = 0; j < history.get(i).length; j++) {
                Label lblNode = new Label(history.get(i)[j]);
                lblNode.setId("historyGrid");
                lblNode.setAlignment(Pos.CENTER);
                historyGrid.add(lblNode, j, i+1);
            }
        }
    }

    private void initialiseNodes() {
        lblHeader = new Label("History");
        historyGrid = new GridPane();
        btnClode = new Button("close");

    }

    private void layoutNodes() {
        lblHeader.setStyle("-fx-font-size: 50; -fx-font-weight: bold");

        Label lblWord = new Label("Word");
        lblWord.setId("historyGridHeader");
        lblWord.setAlignment(Pos.CENTER);
        Label lblAttempts = new Label("Attempts");
        lblAttempts.setId("historyGridHeader");
        lblAttempts.setAlignment(Pos.CENTER);
        Label lblWordFound = new Label("Word found");
        lblWordFound.setId("historyGridHeader");
        lblWordFound.setAlignment(Pos.CENTER);

        historyGrid.add(lblWord, 0, 0);
        historyGrid.add(lblAttempts, 1, 0);
        historyGrid.add(lblWordFound, 2, 0);

        historyGrid.setVgap(-1);
        historyGrid.setHgap(-1);

        btnClode.setPrefWidth(200);
        btnClode.setPrefHeight(35);

        historyGrid.setAlignment(Pos.CENTER);

        this.setStyle("-fx-background-color:  #CCCCCC");

        this.setAlignment(Pos.CENTER);
        this.setSpacing(30);

        this.getChildren().add(lblHeader);
        this.getChildren().add(historyGrid);
        this.getChildren().add(btnClode);
    }

    public Button getBtnClode() {
        return btnClode;
    }
}
