Vagrant.configure("2") do |config|
    config.vm.provider "virtualbox"
    config.vm.box = "alvaro/bionic64"
    config.vm.provision "shell", path: "scripts/provision-go.sh"
    config.vm.provision "shell", path: "scripts/provision-tf.sh"
  end