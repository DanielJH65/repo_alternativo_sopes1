FROM node:22.12-alpine

WORKDIR /app

COPY package*.json ./

RUN npm install

COPY . .

EXPOSE 4005

CMD [ "npm", "run", "dev" ]