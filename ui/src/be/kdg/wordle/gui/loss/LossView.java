package be.kdg.wordle.gui.loss;

import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;

public class LossView extends  VBox {

    private VBox headerLayout;
    private Label lblHeader;
    private Label lblSubText;
    private Button btnMenu;

    public LossView() {
        this.initialiseNodes();
        this.layoutNodes();
    }

    private void initialiseNodes() {

        headerLayout = new VBox();

        lblHeader = new Label("You lost");
        lblSubText = new Label();

        btnMenu = new Button("menu");
    }

    public void addInformation(String correctWord) {
        lblSubText.setText("The correct word was: " + correctWord + ".");
    }

    private void layoutNodes() {
        lblHeader.setStyle("-fx-font-size:  60");

        headerLayout.setAlignment(Pos.CENTER);
        headerLayout.getChildren().addAll(lblHeader, lblSubText);

        this.setStyle("-fx-background-color:  #CCCCCC");

        this.setSpacing(80);
        this.setAlignment(Pos.CENTER);
        this.getChildren().addAll(headerLayout, btnMenu);
    }

    public Button getBtnMenu() {
        return btnMenu;
    }
}
