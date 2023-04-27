from typing import Union
from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI()

class Item(BaseModel):
    name: str
    password: str
    email: str

@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.get("/items/{item_name}")
def read_item(item_name: str, item_email: str, item_password: str):
    return {"item_name": item_name, "item_email": item_email, "item_password": item_password}

@app.put("/items/{item_name}")
def update_item(item_name: str, item_email: str, item_password: str, item: Item):
    return {"item_name": item.name, "item_email": item.email, "item_password": item.password}