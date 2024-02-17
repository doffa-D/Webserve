import cgi
import cgitb

cgitb.enable()
form = cgi.FieldStorage()
print(form["Hello"])

