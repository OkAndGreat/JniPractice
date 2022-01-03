package com.redrock.jnipractice;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
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

        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
        changeHello();
        changeAge();
        tv.setText(hello);

        Toast.makeText(this,"age is " + age,Toast.LENGTH_SHORT).show();
        Toast.makeText(this,"3+4="+sum(3,4),Toast.LENGTH_SHORT).show();
        Toast.makeText(this,"3-4="+minus(3,4),Toast.LENGTH_SHORT).show();

        int[] intArray = new int[3];
        String[] strs = new String[]{"李小龙","李连杰","李元霸"}; // 对象类型的数组
        changeArray(intArray,strs);
        for(int each : intArray){
            Log.d("int数组native修改", "onCreate: " + each);
        }

    }

    /**
     * 手动释放C++层的全局引用
     */
    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @SuppressWarnings("unused")
    //相减俩数供c++调用
    private int minusInJava(int a,int b){
        return a-b;
    }

    /**
     * A native method that is implemented by the 'jnipractice' native library,
     * which is packaged with this application.
     */
    @SuppressWarnings("unused")
    public native String stringFromJNI();

    /**
     *  调用C++完成俩数相加
     */
    public native int sum(int a,int b);

    /**
     * C++层改变String hello字符串
     */
    public native void changeHello();

    /**
     * C++层调用java的方法
     */
    public native int minus(int a,int b);

    /**
     * C++层改变int age
     */
    private static native void changeAge();

    /**
     * C++层修改java基本类型数组和String数组
     */
    private native void changeArray(int[] intArray,String[] strArray);

    /**
     * 传递引用类型对象
     */
    public native void putObject(Student student, String str);

    /**
     * 在C++层创建java的对象
     */
    public native void newObject();

}