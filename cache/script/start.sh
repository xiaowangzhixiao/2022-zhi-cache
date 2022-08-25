if [ ! -d /mydata ]; then
  cp -r /data /mydata
fi
nohup /root/cache_server > /root/run.log 2>&1 &