sudo apt-get update
sudo apt-get install ca-certificates curl make gcc-12 -y
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 12
sudo update-alternatives --config gcc
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update

sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin make -y

#sudo apt-get install --reinstall linux-headers-$(uname -r) -y

#sudo apt install make gcc-12 -y
#sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 12
#sudo update-alternatives --config gcc

git clone https://github.com/DanielJH65/SO1_VAC_DIC_2024_201901108.git

cd SO1_VAC_DIC_2024_201901108/Proyecto1/modules/CPU
make
insmod cpu_201901108.ko
cd ../RAM
make
insmod ram_201901108.ko

cd ../../agent


sudo docker compose up -d