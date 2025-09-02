package com.example.BluetoothSmartBox.Utils;

import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.view.Gravity;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.example.BluetoothSmartBox.R;


/**
 * 自定义等待动画
 */
public class CustomDialog extends Dialog {
    private Context context;
    private int theme;
    private String str = "扫描中...";
    private TextView TvLoading;
    private ImageView IvLoading;

    public CustomDialog(@NonNull Context context) {
        super(context);
        this.context = context;
        Start();
    }
    /**
     * 启动动画
     */
    private void Start(){
        this.setCanceledOnTouchOutside(true); //（不）允许点击其它地方退出
        this.setOnCancelListener((OnCancelListener) (new OnCancelListener() {
            public final void onCancel(DialogInterface it) {
                CustomDialog.this.dismiss();
            }
        }));
        this.setContentView(R.layout.loading_dialog);
        TvLoading = findViewById(R.id.tv_loading_tx);
        TvLoading.setText(str);
        IvLoading = findViewById(R.id.iv_loading);
        // 使用ImageView显示动画
        IvLoading.startAnimation(AnimationUtils.loadAnimation(context, R.anim.loading_animation));
        this.getWindow().getAttributes().gravity = Gravity.CENTER;//居中显示
        this.getWindow().getAttributes().dimAmount = 0.5f;//背景透明度  取值范围 0 ~ 1
    }
    /**
     * 关闭窗口
     */
    public void dismiss() {
        super.dismiss();
    }

}
