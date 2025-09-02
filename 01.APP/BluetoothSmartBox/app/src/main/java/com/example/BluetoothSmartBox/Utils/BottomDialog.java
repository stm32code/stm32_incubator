package com.example.BluetoothSmartBox.Utils;


import static com.example.BluetoothSmartBox.Utils.Common.SCAN_START;

import android.app.Dialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.FrameLayout;
import android.widget.ListView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.coordinatorlayout.widget.CoordinatorLayout;

import com.example.BluetoothSmartBox.Bluetooth.BlueTooth;
import com.example.BluetoothSmartBox.MainActivity;

import com.example.BluetoothSmartBox.R;
import com.example.BluetoothSmartBox.adapter.BlueToothListViewAdapter;
import com.google.android.material.bottomsheet.BottomSheetBehavior;
import com.google.android.material.bottomsheet.BottomSheetDialog;
import com.google.android.material.bottomsheet.BottomSheetDialogFragment;

import java.util.HashMap;
import java.util.List;

public class BottomDialog extends BottomSheetDialogFragment {
    private Context mContext;
    private View view;
    public static ListView matchedListView, scanListView;
    private String TAG = "底部弹窗";
    private Handler handler;

    public BottomDialog(Context context, Handler handler) {
        mContext = context;
        this.handler = handler;
        Common.blueTooth = new BlueTooth(context);
    }

    @NonNull
    @Override
    public Dialog onCreateDialog(@Nullable Bundle savedInstanceState) {
        Log.d(TAG, "onCreateDialog: 创建底部弹出框");
        //返回BottomSheetDialog的实例
        return new BottomSheetDialog(this.getContext());
    }


    @Override
    public void onStart() {
        Log.d(TAG, "onStart: ");
        super.onStart();
        //获取dialog对象
        BottomSheetDialog dialog = (BottomSheetDialog) getDialog();
        //把windowsd的默认背景颜色去掉，不然圆角显示不见
        dialog.getWindow().findViewById(com.google.android.material.R.id.design_bottom_sheet).setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
        //获取diglog的根部局
        FrameLayout bottomSheet = dialog.getDelegate().findViewById(com.google.android.material.R.id.design_bottom_sheet);
        if (bottomSheet != null) {
            //获取根部局的LayoutParams对象
            CoordinatorLayout.LayoutParams layoutParams = (CoordinatorLayout.LayoutParams) bottomSheet.getLayoutParams();
            layoutParams.height = getPeekHeight();
            //修改弹窗的最大高度，不允许上滑（默认可以上滑）
            bottomSheet.setLayoutParams(layoutParams);
            final BottomSheetBehavior<FrameLayout> behavior = BottomSheetBehavior.from(bottomSheet);
            //peekHeight即弹窗的最大高度
            behavior.setPeekHeight(getPeekHeight());
            // 初始为展开状态
            behavior.setState(BottomSheetBehavior.STATE_EXPANDED);

        }

    }

    /**
     * 弹窗高度，默认为屏幕高度的四分之三
     * 子类可重写该方法返回peekHeight
     *
     * @return height
     */
    protected int getPeekHeight() {
        int peekHeight = getResources().getDisplayMetrics().heightPixels;
        //设置弹窗高度为屏幕高度的3/4
        return peekHeight - peekHeight / 4;
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        //mContext = getContext();
        Log.e(TAG, "onCreateView ");
        view = inflater.inflate(R.layout.bluetooth_view, container, false);
        initViews(view);
        return view;
    }

    private void initViews(View view) {
        matchedListView = view.findViewById(R.id.matchedListView);
        scanListView = view.findViewById(R.id.scanListView);
        List<HashMap<String, String>> map = Common.blueTooth.ScanPairBlueTooth();
        matchedListView.setAdapter(new BlueToothListViewAdapter(map, mContext));
        matchedListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                if (map != null) {
                    Common.blueTooth.btConnetOrServer(MainActivity.mHandler, map.get(i).get("btAddress"));
                }
            }
        });


        view.findViewById(R.id.scanButton).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                scanListView.setVisibility(View.VISIBLE);
                handler.sendMessage(handler.obtainMessage(SCAN_START, ""));
            }
        });

    }


    /**
     * 关闭窗口
     */
    public void dismiss() {
        super.dismiss();
    }
}
