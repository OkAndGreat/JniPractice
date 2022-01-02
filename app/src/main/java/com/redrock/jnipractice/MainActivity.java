package com.redrock.jnipractice;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

import com.redrock.jnipractice.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'jnipractice' library on application startup.
    static {
        System.loadLibrary("jnipractice");
    }

    private String hello = "";

    private static int age = 10;

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
//        tv.setText(stringFromJNI());
        changeHello();
        changeAge();
        tv.setText(hello);

        Toast.makeText(this,"age is " + age,Toast.LENGTH_SHORT).show();
        Toast.makeText(this,"3+4="+sum(3,4),Toast.LENGTH_SHORT).show();
        Toast.makeText(this,"3-4="+minus(3,4),Toast.LENGTH_SHORT).show();

    }

    //相减俩数供c++调用
    private int minusInJava(int a,int b){
        return a-b;
    }

    /**
     * A native method that is implemented by the 'jnipractice' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     *  调用C++完成俩数相加
     */
    public native int sum(int a,int b);

    public native void changeHello();

    public native int minus(int a,int b);

    private static native void changeAge();

}