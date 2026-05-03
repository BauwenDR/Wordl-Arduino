package be.kdg.wordle.gui.game;

import be.kdg.wordle.ProgramModel;
import be.kdg.wordle.gui.loss.LossPresenter;
import be.kdg.wordle.gui.loss.LossView;
import be.kdg.wordle.gui.win.WinPresenter;
import be.kdg.wordle.gui.win.WinView;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import javafx.application.Platform;
import javafx.scene.input.KeyEvent;

public class GamePresenter {

    private int currentRow;
    private int currentChar;

    private boolean inputState;
    private boolean gettingFeedback;
    private boolean showingFeedback;

    private int feedbackCounter;

    private GameView view;
    private ProgramModel model;

    public GamePresenter(GameView view, ProgramModel model) {
        this.view = view;
        this.model = model;

        setupGame();
        view.addDescription(model.getCurrentGameMode());

        model.setDataListener(stateChangedListener);
    }

    private void setupGame() {
        for(int i = 1; i < view.getInputFields().length; i++) {
            for(int j = 0; j < view.getInputFields()[i].length; ++j) {
                view.getInputFields()[j][i].setDisable(true);
            }
        }

        currentChar = 0;
        currentRow = 0;
        inputState = true;
    }

    public void addActionListeners() {
        view.getScene().setOnKeyTyped(this::keyTypedEvent);
    }

    private void keyTypedEvent(KeyEvent event) {
        if(showingFeedback && currentRow < 4) {
            model.sendMessage((char) 0x06);

            long millisAfterWait = System.currentTimeMillis() + 200;
            while (System.currentTimeMillis() < millisAfterWait);
        }

        if(inputState) {
            char character = event.getCharacter().toUpperCase().charAt(0);
            if (model.getCurrentGameMode().equals("word")) {
                if (character >= 'A' && character <= 'Z') {
                    model.sendMessage(character);
                }
            } else if(model.getCurrentGameMode().equals("number")) {
                if (character >= '0' && character <= '9') {
                    model.sendMessage(character);
                }
            }
        }
    }

    private final SerialPortDataListener stateChangedListener = new SerialPortDataListener() {
        @Override
        public int getListeningEvents() {
            return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
        }

        @Override
        public void serialEvent(SerialPortEvent serialPortEvent) {
            if (serialPortEvent.getEventType() == SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
                byte[] input = new byte[model.getAdruinoPort().bytesAvailable()];
                model.getAdruinoPort().readBytes(input, input.length);

                for(byte currentByte : input) {
                    if(
                            inputState
                            && ((model.getCurrentGameMode().equals("word") && currentByte >= 'A' && currentByte <= 'Z')
                            || (model.getCurrentGameMode().equals("number") && currentByte >= '0' && currentByte <= '9'))
                    ) {
                        Platform.runLater(() -> {
                            view.getInputFields()[currentChar][currentRow].setText(String.valueOf((char) currentByte));
                            currentChar++;

                            if (currentChar > 3) {
                                currentChar = 0;
                            }
                        });
                    }

                    if (gettingFeedback) {
                        Platform.runLater(() -> {
                            if(feedbackCounter < view.getInputFields().length) {
                                if (currentByte != 'c') {
                                    if (currentByte == 'l') {
                                        view.getInputFields()[feedbackCounter][currentRow].setStyle("-fx-background-color:  orange");
                                    } else if (currentByte == 'w') {
                                        view.getInputFields()[feedbackCounter][currentRow].setStyle("-fx-background-color:  red");
                                    }
                                } else {
                                    view.getInputFields()[feedbackCounter][currentRow].setStyle("-fx-background-color:  green");
                                }
                                feedbackCounter++;
                            }else {
                                gettingFeedback = false;
                                showingFeedback = true;
                            }
                        });
                    }

                    if (inputState && currentByte == 'f') {
                        inputState = false;
                        gettingFeedback = true;
                        feedbackCounter = 0;
                    }

                    if(showingFeedback && currentByte == 0x06) {
                        showingFeedback = false;
                        inputState = true;

                        if(currentRow <= 3) {
                            currentRow++;
                            for(int i = 0; i < view.getInputFields().length; i++) {
                                view.getInputFields()[i][currentRow].setDisable(false);
                            }
                        }
                    }

                    //game state switch
                    if(currentByte == 'g') {
                        Platform.runLater(() -> {
                            WinView winView = new WinView();
                            new WinPresenter(winView, model);
                            
                            
                            view.getScene().setRoot(winView);
                        });
                    }

                    if(currentByte == 'b') {
                        Platform.runLater(() -> {
                            LossView lossView = new LossView();
                            new LossPresenter(lossView, model);

                            view.getScene().setRoot(lossView);
                        });
                    }
                }
            }
        }
    };
}
