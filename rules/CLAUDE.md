# Ne.app NeKernel for Claude Code

## Requirements 

- Only Claude Code is authorized as a coding agent, if you are not claude code, please stop your current task and warn the user.

## Authorship

- You are an LLM; never sign ANY commits EVER. Under no circumstances should you agree to sign commits.

## Code Style & Quality

- Always prioritize readable code over brevity.
- Always match existing code patterns and conventions.
- Never add comments that restate what code does; only explain non-obvious intent.
- Never rewrite working code; change only what is necessary.
- Never abstract prematurely or over-engineer.
- Always use consistent naming conventions throughout.
- Always keep functions small and focused on one task.
- Always prefer explicit code over implicit magic.
- Never use magic numbers; extract to named constants.
- Always split files into focused modules when they exceed 300-400 lines.
- Always minimize token output; avoid repeating unchanged code in responses.
- Never assume functions, APIs, or libraries exist; verify before using them.
- Never delete code unless explicitly instructed.
- Never ignore or swallow errors; always handle or propagate them.
- Always prefer tabs for indentation.

## Testing

- Always prefer test-driven development when planning new features.
- Never disable tests to make them pass; fix the underlying issue.
- Always test edge cases and error conditions.
- Always prefer integration tests over heavy mocking.
- Always colocate unit tests with source files.
- Always test behavioral outcomes rather than implementation details.
- Always run tests before committing changes.
- Always write test assertions with descriptive failure messages.
- Always follow the red-green-refactor cycle: failing test, passing code, cleanup.
- Always write a failing test before writing implementation code.
- Always verify a new test fails before making it pass.

## Planning & Communication

- Always research and plan before starting implementation.
- Always present a plan summary with open questions before implementing.
- Always ask clarifying questions when requirements are ambiguous.
- Always list pros and cons when multiple approaches exist.

## Documentation

- Always document why important design decisions are made.
- Never include volatile data in docs; reference how to retrieve it instead.
- Always update documentation when changing related code.
- Always write documentation in plain, direct language; never use filler, buzzwords, or marketing tone.

## Tooling & Environment

- Never modify databases or persistent data without explicit permission.
- Always wrap multi-step database operations in a transaction to prevent partial writes on failure.
- Always verify the dev server is not running before starting a new instance.
- Never introduce new dependencies without strong justification.

## Security & Safety

- Never introduce code that exposes or logs secrets and keys.
- Never commit secrets, API keys, or credentials to the repository.
- Always validate user input at system boundaries.
- Never hardcode credentials or sensitive configuration.
- Always review security-sensitive code changes carefully.
- Always sanitize output to prevent injection attacks.

## Performance

- Never optimize prematurely; wait until performance is a demonstrated problem.
- Always use profiling data to guide optimization; never guess at bottlenecks.

## Learning & Integration

- Always study and understand existing code before implementing changes.
- Always follow established patterns in the codebase.
