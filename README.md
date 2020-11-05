# cacti-env
Succulents and cacti indoor environment monitoring




#

Start docker-compose:
```commandline
sudo docker-compose up -d
```

Stop docker-compose: 
```
sudo docker-compose down
```

Get list of only running Docker containers: 
```commandline
sudo docker ps
```

Run any command in a running container just knowing its ID (or name):
```commandline
docker exec -it <container_id_or_name> echo "I'm inside the container!"
```

Create new account for mosquitto client:
```commandline
sudo mosquitto_passwd -c /etc/mosquitto/passwd <username>
```

Connect to the influx: 
```
sudo docker exec -it <container_id_or_name> influx
```
Example
```
sudo docker exec -it influxdb influx
```

show databases

use <db_name>

show measurements

drop measurement "name"

select * from temperature where  value = -0.1 limit 10
select * from temperature where  value = -0.1 order by time desc limit 10 

delete from temperature  where time = 1598902818348571313



