from influxdb import InfluxDBClient

client = InfluxDBClient(host=u'127.0.0.1', port=8086, database='cacti')

# query = 'select * from temperature where value >100'
drop_query_mask = 'delete from temperature where time = %s'

point_list_to_delete = [1603203565459649461,
                        1603203627582389631,
                        1603203689705446707]

for point in point_list_to_delete:
    query = drop_query_mask % point
    result = client.query(query)
    print(point, 'OK')

