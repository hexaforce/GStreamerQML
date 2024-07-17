#!/bin/bash

# 引数のチェック
if [ $# -ne 6 ]; then
    echo "Usage: bash $0 <SSID> <PASSPHRASE> <INTERFACE> <GATEWAY_IP> <DHCP_RANGE_START> <DHCP_RANGE_END>"
    echo " "
    echo "Example:"
    echo "bash $0 YourSSID YourPassphrase wlan0 192.168.4.1 192.168.4.2 192.168.4.20"
    exit 1
fi

# hostapdとdnsmasqのインストール確認
if ! command -v hostapd &> /dev/null || ! command -v dnsmasq &> /dev/null; then
    echo "hostapd or dnsmasq is not installed. Installing them now..."
    sudo apt-get update
    sudo apt-get install -y hostapd dnsmasq

    # インストール後に再度確認
    if ! command -v hostapd &> /dev/null || ! command -v dnsmasq &> /dev/null; then
        echo "Failed to install hostapd or dnsmasq. Please install them manually."
        exit 1
    fi
else
    echo "hostapd and dnsmasq are installed and ready."
fi

# hostapdのconfファイル参照を設定
sudo sed -i 's/^#DAEMON_CONF=""/DAEMON_CONF="\/etc\/hostapd\/hostapd.conf"/' /etc/default/hostapd

# 引数の取得
SSID=$1
PASSPHRASE=$2
INTERFACE=$3
GATEWAY_IP=$4
DHCP_RANGE_START=$5
DHCP_RANGE_END=$6

# ネットワークインターフェースの設定リセット
sudo ip addr flush dev $INTERFACE

# ネットワークインターフェースの設定
sudo ip link set $INTERFACE down
sudo ip addr add $GATEWAY_IP/24 dev $INTERFACE
sudo ip link set $INTERFACE up

# hostapd設定ファイルの作成
cat << EOF | sudo tee /etc/hostapd/hostapd.conf
interface=$INTERFACE
driver=nl80211
ssid=$SSID
hw_mode=g
channel=6
wmm_enabled=0
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_passphrase=$PASSPHRASE
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
EOF

# hostapdの設定と起動
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
sudo systemctl start hostapd

# hostapdのステータスチェック
if systemctl is-active --quiet hostapd; then
    echo "hostapd is running."
else
    echo "hostapd failed to start. Please check the configuration."
    exit 1
fi

# dnsmasq設定ファイルの作成
cat << EOF | sudo tee /etc/dnsmasq.conf
interface=$INTERFACE
dhcp-range=$DHCP_RANGE_START,$DHCP_RANGE_END,255.255.255.0,24h
EOF

# dnsmasqの再起動
sudo systemctl restart dnsmasq

# dnsmasqのステータスチェック
if systemctl is-active --quiet dnsmasq; then
    echo "dnsmasq is running."
else
    echo "dnsmasq failed to start. Please check the configuration."
    exit 1
fi

# IPフォワーディングの有効化
sudo sysctl -w net.ipv4.ip_forward=1

# iptablesルールの設定
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo iptables -A FORWARD -i eth0 -o $INTERFACE -m state --state RELATED,ESTABLISHED -j ACCEPT
sudo iptables -A FORWARD -i $INTERFACE -o eth0 -j ACCEPT

echo " "
echo "Setup completed successfully."
echo "SSID: $SSID"
echo "Passphrase: $PASSPHRASE"
echo "Interface: $INTERFACE"
echo "Gateway IP: $GATEWAY_IP"
echo "DHCP Range: $DHCP_RANGE_START - $DHCP_RANGE_END"

# hostapdのステータスを表示
# echo " "
# echo "=== Hostapd Status ==="
# sudo systemctl status hostapd
# sudo journalctl -u hostapd

# dnsmasqのステータスを表示
# echo " "
# echo "=== Dnsmasq Status ==="
# sudo systemctl status dnsmasq
# sudo journalctl -u dnsmasq

# sudo iptables -L -n -v --line-numbers

# sudo iptables -D FORWARD 10
# sudo iptables -D FORWARD 9
