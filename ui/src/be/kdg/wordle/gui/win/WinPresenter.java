package be.kdg.wordle.gui.win;

import be.kdg.wordle.ProgramModel;
import be.kdg.wordle.gui.menu.MenuPresenter;
import be.kdg.wordle.gui.menu.MenuView;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import javafx.application.Platform;

public class WinPresenter {

    private WinView view;
    private ProgramModel model;

    public WinPresenter(WinView view, ProgramModel model) {
        this.view = view;
        this.model = model;

        addEventListeners();
        model.setDataListener(stateChangedListener);
    }

    private void addEventListeners() {
        view.getBtnMenu().setOnAction(e -> model.sendMessage('m'));
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
                    }else if(recievedByte == 'g') {
                    	model.sendMessage((char) 0x06);
                    	
                    	long millisAfterWait = System.currentTimeMillis() + 150;
                        while (System.currentTimeMillis() < millisAfterWait);
                        
                        model.sendMessage('?');
                	}else if(recievedByte-48 < 5) {
                        Platform.runLater(() -> {
                            view.addInformation(recievedByte-48);
                            model.sendMessage('+');
                        });
                    }
                }
            }
        }
    };
}
