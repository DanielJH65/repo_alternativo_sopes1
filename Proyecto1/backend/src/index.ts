import { serve } from '@hono/node-server'
import { Hono } from 'hono'
import { cors } from 'hono/cors'
import mysql from 'mysql2/promise'

const app = new Hono()
app.use('/*', cors())

const config = {
  host: 'localhost',
  user: 'root',
  port: 3306,
  password: 'root',
  database: 'Proyecto1'
}

const connection = await mysql.createConnection(config)

app.get('/', (c) => {
  return c.text('Hello Hono!')
})

app.post('/ram', async (c) => {
  const data = await c.req.json()
  const clientIp = c.req.header('x-forwarded-for') || c.req.header('x-real-ip') || "0.0.0.0"
  console.log(data)
  const { total_ram, free_ram, used_ram, percentage_ram } = data
  const result = await connection.query(
    'INSERT INTO ram (ip, total_ram, free_ram, used_ram, percentage_ram) VALUES (?, ?, ?, ?, ?)',
    [clientIp, total_ram, free_ram, used_ram, percentage_ram]
  )
  return c.json({ message: "recibido" }, 201)
})

app.post('/cpu', async (c) => {
  const data = await c.req.json()
  const clientIp = c.req.header('x-forwarded-for') || c.req.header('x-real-ip') || "0.0.0.0"
  console.log(data)
  const { percentage_cpu, tasks } = data
  const result = await connection.query(
    'INSERT INTO cpu (ip, percentage_cpu) VALUES (?, ?)',
    [clientIp, percentage_cpu]
  )
  await connection.query('DELETE FROM tasks WHERE ip = ?', [clientIp])
  for (const task of tasks) {
    const { pid, name, status, user, father, ram } = task
    await connection.query(
      'INSERT INTO tasks (ip, pid, name, status, user, ram, father) VALUES (?, ?, ?, ?, ?, ?, ?)',
      [clientIp, pid, name, status, user, ram, father]
    )
  }
  return c.json({ message: "recibido" }, 201)
})

const port = 4005
console.log(`Server is running on http://localhost:${port}`)

serve({
  fetch: app.fetch,
  port
})
