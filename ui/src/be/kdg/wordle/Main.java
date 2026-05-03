package be.kdg.wordle;

import be.kdg.wordle.gui.initialise.InitialisePresenter;
import be.kdg.wordle.gui.initialise.InitialiseView;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.util.Objects;

public class Main  extends Application {

    @Override
    public void start(Stage stage) throws Exception {
        stage.setTitle("Worlde Companion App");

        ProgramModel model = new ProgramModel();

       InitialiseView view = new InitialiseView();
       InitialisePresenter presenter = new InitialisePresenter(view, model);

        Scene scene = new Scene(view);
        scene.getStylesheets().add(Objects.requireNonNull(getClass().getResource("/css/program.css")).toExternalForm());

        stage.setScene(scene);
        stage.show();

        stage.setMinWidth(1280);
        stage.setMinHeight(720);
        stage.centerOnScreen();

        //rty and sync after showing window
        presenter.syncWithDevice();

        stage.setOnCloseRequest( e -> {
            if(!(model.getAdruinoPort() == null)) {
                model.getAdruinoPort().removeDataListener();
                model.getAdruinoPort().closePort();
            }
            System.exit(0);
        });
    }
}
