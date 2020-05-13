package com.sjq.houseplayer;

import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class HousePlayer implements SurfaceHolder.Callback {
    static {
        System.loadLibrary("houseplayer");
    }

    private SurfaceHolder surfaceHolder;
    //绘制NDK path surfaceview
    public void setSurfaceView(SurfaceView surfaceView){

        if(null != this.surfaceHolder){
            this.surfaceHolder.removeCallback(this);
        }
        this.surfaceHolder = surfaceView.getHolder();
        this.surfaceHolder.addCallback(this);

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

        this.surfaceHolder = holder;
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void start(String path){

        nativeStart(path,surfaceHolder.getSurface());

    }

    public native void nativeStart(String path, Surface surface);

    public native void sound(String input,String output);
}
