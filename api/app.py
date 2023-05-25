from fastapi import FastAPI, Request, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from bson import ObjectId
import motor.motor_asyncio
import pydantic
from datetime import datetime
from pydantic import BaseModel



app = FastAPI()

origins = [
    
    "http://localhost:5500","http://localhost:5500/client","http://localhost:5500/client/index.html"

]



app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],)

client = motor.motor_asyncio.AsyncIOMotorClient("mongodb+srv://gawaynewright10:capstone2023@cluster0.2xzxpao.mongodb.net/?retryWrites=true&w=majority",tls=True, tlsAllowInvalidCertificates=True)

db = client.DoorEnergyHarvester

pydantic.json.ENCODERS_BY_TYPE[ObjectId]=str



@app.post("/data",status_code=201)
async def newprofile(request:Request):
    PowerObject = await request.json()
    PowerObject ["datetime"] = datetime.now()
    currentsensorvoltage = PowerObject["current"]/1024*5.00
    PowerObject["current"]= (currentsensorvoltage-2.5)/0.185
    PowerObject["voltage"] = PowerObject["voltage"]/1024*5.00
    new_object = await db["Power_data"].insert_one(PowerObject)
    foundObject = await db["Power_data"].find_one({"_id": new_object.inserted_id})
    return foundObject


@app.get("/data")
async def retrive_tanks():
    PowerObjects = await db["Power_data"].find().sort("datetime",-1).to_list(10)
    return PowerObjects[::-1]
