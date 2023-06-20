#!/usr/bin/python

import cgi

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
name = form.getvalue('name')
age = form.getvalue('age')

# Set the content type of the response

print("Content-type:text/html\r\n\r\n")

# Generate the HTML response
print("<html>")
print("<head>")
print("<title>CGI Python Script</title>")
print("</head>")
print("<body>")
print("<h2>Received Data</h2>")
print("<p>Name: {0}</p>".format(name))
print("<p>Age: {0}</p>".format(age))
print("</body>")
print("</html>")

while 1:
    a = 1