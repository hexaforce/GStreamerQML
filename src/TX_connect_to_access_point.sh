#!/bin/bash

# 引数のチェック
if [ $# -ne 3 ]; then
    echo "Usage: bash $0 <WLAN_INTERFACE> <SSID> <PASSWORD>"
    echo " "
    echo "Example:"
    echo "bash $0 wlan0 YourSSID YourPassphrase"
    exit 1
fi

sudo nmcli device wifi connect "YourSSID" password "YourPassphrase" ifname "wlan0"
sudo nmcli device wifi connect YourSSID password YourPassphrase ifname wlan0



# 引数の取得
WLAN_INTERFACE=$1
SSID=$2
PASSWORD=$3

# hostapdを停止する
sudo systemctl stop hostapd

# 既存の同名SSID接続設定を削除する
for i in {0..12}; do
    sudo nmcli connection delete "$SSID $i" 2>/dev/null
done
sudo nmcli connection delete "$SSID" 2>/dev/null

# インターフェースを起動する
echo "Bringing up the interface $WLAN_INTERFACE..."
sudo ip link set "$WLAN_INTERFACE" up

# WiFiに接続する
echo "Connecting to $SSID..."
sudo nmcli device wifi connect "$SSID" password "$PASSWORD" ifname "$WLAN_INTERFACE"

# 接続結果の確認
if [ $? -eq 0 ]; then
    echo "Successfully connected to $SSID!"
    exit 0
else
    echo "Failed to connect to $SSID."
    nmcli connection show
    exit 1
fi

nmcli connection show