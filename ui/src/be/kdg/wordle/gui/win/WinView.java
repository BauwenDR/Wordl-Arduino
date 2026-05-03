package be.kdg.wordle.gui.win;

import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;

public class WinView extends VBox {

    private VBox headerLayout;
    private Label lblHeader;
    private Label lblSubText;
    private Button btnMenu;

    public WinView() {
        this.initialiseNodes();
        this.layoutNodes();
    }

    private void initialiseNodes() {

        headerLayout = new VBox();

        lblHeader = new Label("Winner");
        lblSubText = new Label();

        btnMenu = new Button("menu");
    }

    public void addInformation(int attempts) {
        lblSubText.setText("You guessed the correct word in " + attempts + " attempts!");
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
