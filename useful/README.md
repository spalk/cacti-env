A list of commonly used commands
================================


### Docker

| Command | Description |
| ------- | ----------- |
| `sudo docker-compose up -d` | Start docker-compose |
| `sudo docker-compose down` | Stop docker-compose |
| `sudo docker ps` | Get list of only running Docker containers |
| `docker exec -it <container_id_or_name> echo "I'm inside the container!"` | Run any command in a running container just knowing its ID (or name) |


### Mosquitto

| Command | Description |
| ------- | ----------- |
| `sudo mosquitto_passwd -c /etc/mosquitto/passwd <username>` | Create new account for mosquitto client |


### InfluxDB

| Command | Description |
| ------- | ----------- |
| `sudo docker exec -it <container_id_or_name> influx` | Connect to the influx in Docker |
| `sudo docker exec -it influxdb influx` | example |
| `show databases` | Show existing databases |
| `use <db_name>` | Make db active |
| `show measurements` | Show existing measurements |
| `drop measurement "name"` | Delete measurement |

#### InfluxDB request examples:
- `select * from temperature where  value = -0.1 limit 10` 
- `select * from temperature where  value = -0.1 order by time desc limit 10`  
- `delete from temperature  where time = 1598902818348571313` 





