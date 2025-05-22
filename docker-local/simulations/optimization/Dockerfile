# Using a recent stable Debian image:
FROM debian:bookworm

# Set environment variables (optional, but good practice for Python)
ENV PYTHONUNBUFFERED=1

# Update the package list and install Python3 and pip
# apt-get for Debian/Ubuntu
RUN apt-get update && \
    apt-get install -y python3 python3-pip && \
    rm -rf /var/lib/apt/lists/* # Clean up apt cache to reduce image size

# Set the base working directory inside the container
WORKDIR /app

# Copy requirements.txt from your local ./opt_ext/ subfolder to /app in the container
# Assuming your requirements.txt is located at ./opt_ext/requirements.txt relative to the Dockerfile
COPY opt_ext/requirements.txt .

# Install the dependencies, including Gurobi
# --break-system-packages might still be needed depending on Python version on Debian/Ubuntu, but try without first.
# If it fails, add --break-system-packages back here.
RUN pip install --break-system-packages -r requirements.txt

# Copy the entire local ./opt_ext/ subfolder into the container at /app/opt_ext
COPY opt_ext /app/opt_ext

# Change the working directory inside the container to where the code is copied
WORKDIR /app/opt_ext

# Install the package itself in editable mode from the current directory (/app/opt_ext)
# --break-system-packages might still be needed depending on Python version, add if install fails.
RUN pip install --break-system-packages -e .

# Note: The gurobi.lic file at ~/optimization/gurobi.lic will NOT be copied into the image.
# You MUST mount the gurobi.lic file when running the container (the normal way).

# Optional: Define the command that runs when the container starts
# Replace 'your_main_script.py' with the actual script or module you want to run from /app/opt_ext
# CMD ["python3", "your_main_script.py"]

# Optional: Expose a port if your application is a network service
# EXPOSE 80
