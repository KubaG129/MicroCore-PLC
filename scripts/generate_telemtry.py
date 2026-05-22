import csv
import random
import os
def generate_industrial_data(filename="telemtry.csv", data_points = 100):
    print(f"[PYTHON] Generating {data_points} telemetry data points")
    target_dir = os.path.dirname(os.path.abspath(__file__))
    filepath = os.path.join(target_dir, "..", filename)

    base_temperature = 22.0

    with open(filepath, mode='w', newline='') as file:
        writer = csv.writer(file, delimiter=";")
        writer.writerow(["STEP", "AI_Temperature"])

        for step in range(data_points):
            if step < 50:
                temperature = base_temperature + random.uniform(-0.2, 0.2)
            else:
                base_temperature += 0.8
                temperature = base_temperature + random.uniform(-0.1, 0.1)
            writer.writerow([step, round(temperature,2)])

    print(f"[PYTHON] Generated {data_points} telemetry data points and saved to {os.path.abspath(filepath)}")
if __name__ == "__main__":
    generate_industrial_data(data_points = 100)