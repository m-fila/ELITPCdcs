curl -k --silent --show-error --fail \
    --header "PRIVATE-TOKEN: ${REPO_KEY}" \
    --data name="${CI_COMMIT_TAG}" \
    --data tag_name="${CI_COMMIT_TAG}" \
    --data description="$(cat CHANGELOG.md)" \
    --request POST "${CI_API_V4_URL}/projects/${CI_PROJECT_ID}/releases"
