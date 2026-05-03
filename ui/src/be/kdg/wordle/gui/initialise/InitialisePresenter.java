package be.kdg.wordle.gui.initialise;

import be.kdg.wordle.ProgramModel;
import be.kdg.wordle.gui.connected.ConnectedPresenter;
import be.kdg.wordle.gui.connected.ConnectedView;
import com.fazecast.jSerialComm.SerialPort;
import javafx.application.Platform;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.value.ChangeListener;

public class InitialisePresenter {

    private InitialiseView view;
    private ProgramModel model;

    private Thread pairThread = new Thread(this::pair);
    private BooleanProperty deviceSynced;

    public InitialisePresenter(InitialiseView view, ProgramModel model) {
        this.view = view;
        this.model = model;

        deviceSynced = new SimpleBooleanProperty();
        deviceSynced.addListener(syncedChangeListener);
    }

    private final ChangeListener<? super Boolean> syncedChangeListener = (observableValue, oldValue, newValue) -> Platform.runLater(() -> {
        if(newValue) {
            //switch to connected view
            changeToConnectedView();
        }
    });

    private void changeToConnectedView() {
        ConnectedView connectedView = new ConnectedView();
        new ConnectedPresenter(connectedView, model);
        view.getScene().setRoot(connectedView);
        deviceSynced.removeListener(syncedChangeListener);
    }

    public void syncWithDevice() {
        pairThread.start();
    }

    private void pair() {
        deviceSynced.set(false);

        while(!deviceSynced.get() && !Thread.currentThread().isInterrupted()) {
            for (SerialPort port : SerialPort.getCommPorts()) {

                port.closePort();
                if(port.openPort()) {
                    long millisAfterWait = System.currentTimeMillis() + 3000;
                    while (System.currentTimeMillis() < millisAfterWait);

                    byte[] requestPairing = {'?'};
                    port.writeBytes(requestPairing, 1);

                    millisAfterWait = System.currentTimeMillis() + 500;
                    while (System.currentTimeMillis() < millisAfterWait) ;

                    if(port.bytesAvailable() > 0) {
                        while(port.bytesAvailable() > 0) { //flush buffer
                            byte[] requestAnswer = new byte[port.bytesAvailable()];
                            port.readBytes(requestAnswer, port.bytesAvailable());
                        }

                        byte[] confirmPairing = {'+'};
                        port.writeBytes(confirmPairing, 1);

                        millisAfterWait = System.currentTimeMillis() + 500;
                        while (System.currentTimeMillis() < millisAfterWait);

                        if(port.bytesAvailable() > 0) {
                            byte[] pairAnswer = new byte[port.bytesAvailable()];
                            port.readBytes(pairAnswer, port.bytesAvailable());

                            if(pairAnswer[0] == 0x06) {
                                model.setAdruinoPort(port);
                                deviceSynced.set(true);
                            }
                        }
                    }
                }else {
                    port.closePort();
                }
            }
        }
    }
}