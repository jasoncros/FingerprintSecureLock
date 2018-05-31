package com.example.android.smartlock;

import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.example.android.smartlock.R;

import java.util.UUID;

public class ControlActivity extends AppCompatActivity {

    Button btnUnlock, btnLock;
    private static final int STATE_DISCONNECTED = 0;
    private static final int STATE_CONNECTING = 1;
    private static final int STATE_CONNECTED = 2;
    BluetoothDevice mBluetoothDevice;
    BluetoothGatt mBluetoothGatt;
    BluetoothGattCharacteristic characteristicRX;
    BluetoothGattCharacteristic characteristicTX;
    static final UUID HM_RX_TX = UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb");
    static final UUID CLIENT_CHARACTERISTIC_CONFIG = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control);

        //Gets the BluetoothDevice from DeviceListActivity and connects to BluetoothGatt
        mBluetoothDevice = getIntent().getExtras().getParcelable(DeviceListActivity.EXTRA_DEVICE);
        mBluetoothGatt = mBluetoothDevice.connectGatt(this, false, mGattCallback);

        btnLock = (Button) findViewById(R.id.but_lock);
        btnUnlock = (Button) findViewById(R.id.but_unlock);

        btnUnlock.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                sendSignal("u\n");
            }
        });
        btnLock.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                sendSignal("l\n");
            }
        });
    }

    //Writes Characteristic to Arduino Bluetooth module
    void sendSignal(String str) {
        byte[] tx = str.getBytes();
        characteristicTX.setValue(tx);
        mBluetoothGatt.writeCharacteristic(characteristicTX);
        setCharacteristicNotification(characteristicRX, true);
    }

    // Fast way to call Toast
    private void showToast(String message) {
        Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT).show();
    }

    //Checks the state of the BluetoothGatt connection
    private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {

        @Override
        public void onConnectionStateChange(final BluetoothGatt gatt, final int status, final int newState) {
            // this will get called when a device connects or disconnects
            System.out.println(newState);
            switch (newState) {
                case STATE_DISCONNECTED:
                    ControlActivity.this.runOnUiThread(new Runnable() {
                        public void run() {
                            showToast("Device Disconnected");
                            Intent intent = new Intent(ControlActivity.this, DeviceListActivity.class);
                            startActivity(intent);
                        }
                    });
                    break;
                case STATE_CONNECTING:
                    showToast("Pairing...");
                case STATE_CONNECTED:
                    ControlActivity.this.runOnUiThread(new Runnable() {
                        public void run() {
                            showToast("Device Paired");
                            // Attempts to discover services after successful connection.
                            showToast("Attempting to start service discovery:" + mBluetoothGatt.discoverServices());
                        }
                    });

                    // discover services and characteristics for this device
                    mBluetoothGatt.discoverServices();

                    break;
                default:
                    ControlActivity.this.runOnUiThread(new Runnable() {
                        public void run() {
                            showToast("We encountered an unknown state, uh oh");
                        }
                    });
                    break;
            }
        }

        //Called when BluetoothGatt.discoveredServices() is called
        @Override
        public void onServicesDiscovered(final BluetoothGatt gatt, final int status) {
            // this will get called after the client initiates a BluetoothGatt.discoverServices() call
            ControlActivity.this.runOnUiThread(new Runnable() {
                public void run() {
                    showToast("Device services have been discovered");
                    if (status == BluetoothGatt.GATT_SUCCESS) {
                        for (BluetoothGattService gattService : mBluetoothGatt.getServices()) {
                            //get characteristic when UUID matches RX/TX UUID
                            characteristicTX = gattService.getCharacteristic(HM_RX_TX);
                            characteristicRX = gattService.getCharacteristic(HM_RX_TX);
                        }
                    } else {
                        showToast("onServicesDiscovered received: " + status);
                    }
                }
            });
        }
    };

    public void setCharacteristicNotification(BluetoothGattCharacteristic characteristic, boolean enabled) {
        this.mBluetoothGatt.setCharacteristicNotification(characteristic, enabled);
        if (HM_RX_TX.equals(characteristic.getUuid())) {
            BluetoothGattDescriptor descriptor = characteristic.getDescriptor(CLIENT_CHARACTERISTIC_CONFIG);
            descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
            this.mBluetoothGatt.writeDescriptor(descriptor);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
