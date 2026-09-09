import boto3
import json
import psycopg2
import os

def json_to_sql(data):
    # print(data)
    data = data['payload']
    return f"({data['pm1']}, {data['pm25']}, {data['pm10']}, {data['co2']}, {data['voc']}, {data['temp']}, {data['hum']}, {data['ch2o']}, {data['co']}, {data['o3']}, {data['no2']}, {data['h2s']}, '{data['timestamp']}')"


def lambda_handler(event, context):
    dynamodb = boto3.resource('dynamodb') # Replace with your region
    table = dynamodb.Table('iaq_dynamodb')
    dev_id = 'iaq_dev'
    try:
        response = table.query(
            KeyConditionExpression= boto3.dynamodb.conditions.Key('dev_id').eq(dev_id)
        )
        print("response", response)
        items = response['Items']
    except Exception as e:
        items = []
    # print(items)
    
    if len(items) == 0:
        return {
            'statusCode': 200,
            'body': {"status": "success", "message": "No items found"}
        }
    sql_commands = "INSERT INTO iaq_sensor_iaq_dev (pm1, pm25, pm10, co2, voc, temp, hum, ch2o, co, o3, no2, h2s, timestamp) VALUES"
    for item in items:
        # print(item)
        sql_commands += " " + json_to_sql(item) + ","
    print(sql_commands) 
    sql_commands = sql_commands[:-1] + ";"

    try:
        conn = psycopg2.connect(
            host='iaq-database-1.ctwwki6wijyo.eu-north-1.rds.amazonaws.com',
            port=5432,
            database='postgres',
            user='iaq_postgres',
            password='iaq_1234567890',
            sslmode='verify-full',
            sslrootcert='global-bundle.pem',
            
        )
        cur = conn.cursor()
        cur.execute(sql_commands)
        conn.commit()
        cur.close()
        conn.close()
    except Exception as e:
        return {
            'statusCode': 500,
            'body': json.dumps(str(e))
        }

    with table.batch_writer() as batch:
        for item in items:
            try:
                # Use the native types for the item partition key
                batch.delete_item(
                    Key={
                        'dev_id': dev_id,
                        'timestamp': item['timestamp']
                    }
                )
            except Exception as e:
                print(f"An error occurred: {e}")
    
    return {
        'statusCode': 200,
        'body': {"status": "success"}
    }

if __name__ == "__main__":
    print(lambda_handler(None, None))