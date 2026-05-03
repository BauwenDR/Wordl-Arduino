package be.kdg.wordle.gui.connected;

import be.kdg.wordle.ProgramModel;
import be.kdg.wordle.gui.menu.MenuPresenter;
import be.kdg.wordle.gui.menu.MenuView;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import javafx.application.Platform;

public class ConnectedPresenter {

    private ConnectedView view;
    private ProgramModel model;

    public ConnectedPresenter(ConnectedView view, ProgramModel model) {
        this.view = view;
        this.model = model;

        this.addEventlisteners();
        model.setDataListener(stateChangedListener);
    }

    private void addEventlisteners() {
        view.getBtnStart().setOnAction(e -> model.sendMessage('s'));
    }

    private SerialPortDataListener stateChangedListener = new SerialPortDataListener() {
        @Override
        public int getListeningEvents() {
            return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
        }

        @Override
        public void serialEvent(SerialPortEvent serialPortEvent) {
            if(serialPortEvent.getEventType() == SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
                byte[] input = new byte[model.getAdruinoPort().bytesAvailable()];
                model.getAdruinoPort().readBytes(input, input.length);

                for(byte recievedByte : input) {
                    if(recievedByte == 0x06) {
                        Platform.runLater(() -> {
                            MenuView menuView = new MenuView();
                            new MenuPresenter(menuView, model);

                            view.getScene().setRoot(menuView);
                        });
                    }
                }
            }
        }
    };
}
