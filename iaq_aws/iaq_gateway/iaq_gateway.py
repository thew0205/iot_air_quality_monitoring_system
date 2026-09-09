import json
import os 

import datetime
import psycopg2

def json_serial(obj):
    if isinstance(obj, datetime.datetime):
        return obj.isoformat()
    raise TypeError("Type %s not serializable" % type(obj))

def lambda_handler(event, context):
    
    conn = None
    body_paload = {}
    try:
        start_period = event['queryStringParameters']['start_period']
        end_period = event['queryStringParameters']['end_period']
        # start_period = "2026-09-03 04:00:00"
        # end_period = "2026-09-04 14:30:00"
        client_id = "iaq_sensor_iaq_dev"
        
        conn = psycopg2.connect(
            host='iaq-database-1.ctwwki6wijyo.eu-north-1.rds.amazonaws.com',
            port=5432,
            database='postgres',
            user='iaq_postgres',
            password=os.getenv('PASSWORD'),
            sslmode='verify-full',
            sslrootcert='global-bundle.pem',

        )
        
        cur = conn.cursor()
        cur.execute(f"SELECT * FROM {client_id} WHERE \"timestamp\" BETWEEN '{start_period}'::timestamp AND '{end_period}'::timestamp;")
        values = cur.fetchall()
        
        cur.close()
    
        
        body_paload =  json.dumps(values, default=json_serial)
        
    except Exception as e:
        print(e)
        body_paload =  json.dumps(str(e))
    finally:
        if conn:
            conn.close()
    return {
            'statusCode': 200,
            'body': body_paload
        }



if __name__ == "__main__":
    print(lambda_handler(None, None))