package com.example.BluetoothSmartBox.bean;

public class Send {

    private Integer cmd;
    private Integer flage;

    public void setCmd(Integer cmd) {
        this.cmd = cmd;
    }

    public void setFlage(Integer flage) {
        this.flage = flage;
    }

    public void setTemp_min(Integer temp_min) {
        this.temp_min = temp_min;
    }

    public void setTemp_max(Integer temp_max) {
        this.temp_max = temp_max;
    }

    public void setLed(Integer led) {
        this.led = led;
    }

    public void setCold(Integer cold) {
        this.cold = cold;
    }

    public void setHot(Integer hot) {
        this.hot = hot;
    }

    private Integer temp_min;
    private Integer temp_max;

    private Integer led;
    private Integer cold;
    private Integer hot;




}
