print('Content-Type: text/html', end='\r\n\r\n')
print()
import sys
import os

print("Hi")
# print(os.environ)

a = '''

<!DOCTYPE html>
<html>
<head>
  <title>Post Name and State</title>
</head>
<body>
  <h2>Post Name and State</h2>
  <form action="say.py" method="post">
    <label for="name">Name:</label>
    <input type="text" id="name" name="name" required><br><br>

    <label for="state">State:</label>
    <select id="state" name="state" required>
      <option value="">Select a state</option>
      <option value="Alabama">Alabama</option>
      <option value="Alaska">Alaska</option>
      <option value="Arizona">Arizona</option>
      <!-- Add more options for other states -->
    </select><br><br>

    <input type="submit" value="Submit">
  </form>
</body>
</html>


'''

print(a, end='')