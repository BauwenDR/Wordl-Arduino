package be.kdg.wordle.gui.menu;

import be.kdg.wordle.ProgramModel;
import be.kdg.wordle.gui.game.GamePresenter;
import be.kdg.wordle.gui.game.GameView;
import be.kdg.wordle.gui.history.HistoryPresenter;
import be.kdg.wordle.gui.history.HistoryView;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

import java.util.LinkedList;
import java.util.Objects;

public class MenuPresenter {

    private MenuView view;
    private ProgramModel model;

    private boolean readingHistory;
    private LinkedList<String[]> gameHistory;
    private String[] currentHistoryLine;
    private int currentIndex;

    public MenuPresenter(MenuView view, ProgramModel model) {
        this.view = view;
        this.model = model;

        readingHistory = false;

        this.addEventListeners();
    }

    private void addEventListeners() {

        view.getBtnWordGame().setOnAction(e -> {
            if(!readingHistory) {
                model.sendMessage('w');
            }
        });
        view.getBtnNumberGame().setOnAction(e ->{
            if(!readingHistory) {
                model.sendMessage('n');
            }
        });

        view.getBtnHistory().setOnAction(e-> {
            if(!readingHistory) {
                model.sendMessage('h');
            }
        });

        model.setDataListener(stateChangedListener);
    }

    private final SerialPortDataListener stateChangedListener = new SerialPortDataListener() {
        @Override
        public int getListeningEvents() {
            return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
        }

        @Override
        public void serialEvent(SerialPortEvent serialPortEvent) {
            if(serialPortEvent.getEventType() == SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
                byte[] input = new byte[model.getAdruinoPort().bytesAvailable()];
                model.getAdruinoPort().readBytes(input, input.length);
                
                for(byte receivedByte : input) {
                    if(receivedByte == 'w' && !readingHistory) {
                        Platform.runLater(() -> {
                            model.setCurrentGameMode("word");
                            GameView gameView = new GameView();
                            GamePresenter presenter = new GamePresenter(gameView, model);

                            view.getScene().setRoot(gameView);
                            presenter.addActionListeners();
                        });
                    }

                    if(receivedByte == 'n' && !readingHistory) {
                        Platform.runLater(() -> {
                            model.setCurrentGameMode("number");
                            GameView gameView = new GameView();
                            GamePresenter presenter = new GamePresenter(gameView, model);

                            view.getScene().setRoot(gameView);
                            presenter.addActionListeners();
                        });
                    }

                    if(readingHistory) {
                        if(receivedByte == 0x03) {
                            Platform.runLater(() -> {
                                readingHistory = false;

                                HistoryView historyView = new HistoryView();
                                new HistoryPresenter(historyView, model, gameHistory);

                                Stage historyStage = new Stage(StageStyle.DECORATED);
                                historyStage.initOwner(view.getScene().getWindow());;
                                historyStage.initModality(Modality.APPLICATION_MODAL);

                                Scene historyScene = new Scene(historyView);
                                historyScene.getStylesheets().add(Objects.requireNonNull(getClass().getResource("/css/program.css")).toExternalForm());
                                historyStage.setTitle("Game History");
                                historyStage.setWidth(800);
                                historyStage.setHeight(600);
                                historyStage.setScene(historyScene);
                                historyStage.centerOnScreen();
                                historyStage.setResizable(false);
                                historyStage.show();
                            });
                        }else {
                            if(currentIndex == 0) {
                                currentHistoryLine = new String[3];
                                currentHistoryLine[0] = "";
                                currentHistoryLine[1] = "";
                                currentHistoryLine[2] = "";
                            }

                            if(currentIndex < 4) {
                                currentHistoryLine[0] += (char) receivedByte;
                            }

                            if(currentIndex == 4) {
                                currentHistoryLine[1] += (char) receivedByte;
                            }

                            if(currentIndex == 5) {
                                if(receivedByte == '1') {
                                    currentHistoryLine[2] = "true";
                                }else {
                                    currentHistoryLine[2] = "false";
                                }
                            }

                            currentIndex++;
                            if(currentIndex > 5) {
                                gameHistory.push(currentHistoryLine);
                                currentIndex = 0;
                            }
                        }
                    }

                    if(receivedByte == 0x02) {
                        readingHistory  = true;
                        gameHistory = new LinkedList<>();
                        currentIndex = 0;
                    }
                }
            }
        }
    };
}
