FROM nodered/node-red:latest
RUN for addonnodes in \
node-red-dashboard \
node-red-contrib-influxdb \
node-red-contrib-boolean-logic \
node-red-node-rbe \
node-red-configurable-ping \
node-red-node-openweathermap \
node-red-node-email \
node-red-node-google \
node-red-node-ping \
; do \
npm install ${addonnodes} ;\
done;
