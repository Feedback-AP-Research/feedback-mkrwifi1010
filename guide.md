
What is the Secret Tab?
Do you have any sensitive data (passwords, API keys...) you don’t want to share publicly?
Follow these steps:

    In your sketch replace the strings containing sensitive data by writing SECRET_NAMEYOUWANT.
    For instance if your code is char ssid[] = "your ssid"; you should replace it with char ssid[] = SECRET_SSID;. Make sure all these identifiers are in caps.
    If an empty string is an accepted value for the identifier, use SECRET_OPTIONAL_NAMEYOUWANT.
    The Secret tab will be automatically created after adding a secret identifier in your .ino file, return here and fill the fields with the sensitive values.
    Upload your sketch!

If you share this sketch with someone else, the values on this tab will be blank.
All the values you enter will be considered a string, some special characters will be automatically escaped.
SECRET_SSID
SECRET_PASS

