package be.kdg.wordle;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;

public class ProgramModel {

    private SerialPort adruinoPort;
    private String currentGameMode = "words";

    public void sendMessage(char message) {
        byte[] sendMessage = {(byte) message};
        adruinoPort.writeBytes(sendMessage, sendMessage.length);
    }

    public boolean sendMessageAndCheckReply(char message, char expectedReply, int replyDelay) {
        if (adruinoPort == null) {
            return false;
        }

        byte[] messageToSend = {(byte) message};

        adruinoPort.writeBytes(messageToSend, 1);

        long millisAfterWait = System.currentTimeMillis() + replyDelay;
        while (System.currentTimeMillis() < millisAfterWait) ;

        if (adruinoPort.bytesAvailable() > 0) {
            byte[] requestAnswer = new byte[adruinoPort.bytesAvailable()];
            adruinoPort.readBytes(requestAnswer, adruinoPort.bytesAvailable());

            return  requestAnswer[0] == expectedReply;
        }
        return false;
    }

    public void setDataListener(SerialPortDataListener dataListener) {
        adruinoPort.removeDataListener();
        adruinoPort.addDataListener(dataListener);
    }

    public SerialPort getAdruinoPort() {
        return adruinoPort;
    }

    public void setAdruinoPort(SerialPort adruinoPort) {
        this.adruinoPort = adruinoPort;
    }

    public String getCurrentGameMode() {
        return currentGameMode;
    }

    public void setCurrentGameMode(String currentGameMode) {
        this.currentGameMode = currentGameMode;
    }
}
