package be.kdg.wordle.gui.history;

import be.kdg.wordle.ProgramModel;
import javafx.stage.Stage;

import java.util.Collections;
import java.util.LinkedList;

public class HistoryPresenter {

    private HistoryView view;
    private ProgramModel model;

    public HistoryPresenter(HistoryView view, ProgramModel model, LinkedList<String[]> history) {
        this.view = view;
        this.model = model;

        Collections.reverse(history);

        view.fillHistory(history);

        this.addActionListeners();
    }

    private void addActionListeners() {
        view.getBtnClode().setOnAction(e -> {
            Stage stage = (Stage) view.getScene().getWindow();
            stage.close();
        });
    }
}
