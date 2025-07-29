ssh cian@archserver 'echo "1930" | sudo -S rm -rf ~/Documents/cpp/emws && mkdir -p ~/Documents/cpp/emws'

scp \
  ~/Documents/cpp/emws/CMakeLists.txt \
  ~/Documents/cpp/emws/CMakePresets.json \
  cian@archserver:~/Documents/cpp/emws/

scp -r \
  ~/Documents/cpp/emws/resources \
  ~/Documents/cpp/emws/scripts \
  ~/Documents/cpp/emws/src \
  ~/Documents/cpp/emws/tests \
  cian@archserver:~/Documents/cpp/emws/

ssh cian@archserver 'find ~/Documents/cpp/emws/scripts -type f -name "*.sh" -exec chmod +x {} \;'

ssh cian@archserver 'mkdir -p ~/Documents/cpp/emws/build ~/Documents/cpp/emws/results'

ssh cian@archserver 'cd ~; cd ~/Documents/cpp/emws'
