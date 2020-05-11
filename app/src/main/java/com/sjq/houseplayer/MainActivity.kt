package com.sjq.houseplayer

import android.Manifest
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.WindowManager
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class MainActivity : AppCompatActivity() {

    var surfaceView: SurfaceView?=null
    var player:HousePlayer?=null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_main)
        surfaceView = findViewById(R.id.sf_view) as SurfaceView
        player = HousePlayer()
        player?.setSurfaceView(surfaceView)
        bt_play.setOnClickListener {
            PermissionTool.getInstance().chekPermissions(this, arrayOf(
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE),object :PermissionTool.IPermissionsResult{
                override fun passPermissons() {
                    open(surfaceView!!)
                }

                override fun forbitPermissons() {
                    Toast.makeText(this@MainActivity,"reject perssion apply", Toast.LENGTH_LONG).show()
                }

            })
        }

        // Example of a call to a native method
//        sample_text.text = stringFromJNI()
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    external fun stringFromJNI(): String
//
//    companion object {
//
//        // Used to load the 'native-lib' library on application startup.
//        init {
//            System.loadLibrary("houseplayer")
//        }
//    }

    fun open(view:SurfaceView){

        var file = File(Environment.getExternalStorageDirectory().absolutePath + "/hero.mp4")
        if(file.exists()){
            player!!.start(file.absolutePath)
        }

    }
}
