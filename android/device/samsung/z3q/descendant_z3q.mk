# Inherit full device configuration
$(call inherit-product, device/samsung/z3q/full_z3q.mk)

# Inherit some common Descendant stuff
$(call inherit-product, vendor/descendant/config/common_full_phone.mk)

# Product IDs
PRODUCT_NAME := descendant_z3q
